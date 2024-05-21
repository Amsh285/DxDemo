#include "dsrpch.h"
#include "Debug.h"

#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"

namespace dsr
{
	std::shared_ptr<dsr::directX::Direct3dDevice> Debug::Device;
	std::shared_ptr<dsr::scenesystem::SceneManager> Debug::SceneManager;

	std::unordered_map<uint32_t, dsr::ecs::Entity> Debug::m_sceneDebugEntityMap;
	std::vector<DebugLineData> Debug::m_lineData;

	static constexpr uint32_t floatsPerLineVertex = 7;
	static constexpr uint32_t maxLineCount = 100;
	static constexpr uint32_t maxLineByteSize = sizeof(float) * floatsPerLineVertex * 2 * maxLineCount;

	DsrResult Debug::DrawRay(
		const DirectX::XMVECTOR& rayOrigin,
		const DirectX::XMVECTOR& rayDirection,
		const DirectX::XMFLOAT4& color,
		const float& offset,
		const float& scale,
		const std::chrono::duration<float, std::milli> expiration)
	{
		using namespace DirectX;

		XMVECTOR dir = XMVector3Normalize(rayDirection);
		XMVECTOR offsetOrigin = XMVectorAdd(rayOrigin ,XMVectorScale(dir, offset));
		XMVECTOR end = XMVectorAdd(offsetOrigin, XMVectorScale(dir, scale));

		XMFLOAT3 from, to;
		XMStoreFloat3(&from, offsetOrigin);
		XMStoreFloat3(&to, end);

		return DrawLine(from, to, color, expiration);
	}

	DsrResult Debug::DrawLine(
		const DirectX::XMFLOAT3& from,
		const DirectX::XMFLOAT3& to,
		const DirectX::XMFLOAT4& color,
		const std::chrono::duration<float, std::milli>& expiration)
	{
		using namespace dsr::directX;
		using namespace dsr::ecs;

		if (m_lineData.size() >= maxLineCount)
			return DsrResult("Maximum numbers of debuglines.", 301);

		std::optional<uint32_t> currentSceneId = SceneManager->GetActiveSceneId();

		if (!currentSceneId.has_value())
			return DsrResult("No Scene active.", 302);

		auto it = m_sceneDebugEntityMap.find(currentSceneId.value());
		Entity debugLinesEntity;

		std::shared_ptr<LineListComponent> component;

		if (it != m_sceneDebugEntityMap.end())
		{
			debugLinesEntity = it->second;
			component = SceneManager->GetComponentFrom<LineListComponent>(currentSceneId.value(), debugLinesEntity);
		}
		else
		{
			debugLinesEntity = SceneManager->CreateNewEntity();
			m_sceneDebugEntityMap[currentSceneId.value()] = debugLinesEntity;

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = maxLineByteSize;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			std::variant<Direct3dBuffer, dsr_error> createBufferResult = Direct3dBuffer::CreateBuffer(Device, bufferDesc, 0, std::nullopt);

			if (std::holds_alternative<dsr_error>(createBufferResult))
			{
				const dsr_error& error = std::get<dsr_error>(createBufferResult);
				return DsrResult(error.what(), 300);
			}

			Direct3dShaderInputLayout inputLayout;
			inputLayout.AddVector3f("POSITION");
			inputLayout.AddVector4f("COLOR");

			SceneManager->AddComponent<dsr::ecs::TransformComponent>(currentSceneId.value(), debugLinesEntity);

			component = SceneManager->AddComponent<LineListComponent>(currentSceneId.value(), debugLinesEntity);
			component->SetVertexShaderInputLayout(inputLayout);
			component->SetVertexBuffer(std::get<Direct3dBuffer>(createBufferResult));
			component->SetVertexCount(0);
		}

		DebugLineData lineData;
		lineData.From = from;
		lineData.To = to;
		lineData.Color = color;
		lineData.SceneId = currentSceneId.value();
		lineData.ExpirationDate = std::chrono::steady_clock::now() + expiration;
		m_lineData.push_back(lineData);

		UpdateDebugLines(currentSceneId.value(), component);

		return DsrResult::Success("Drawline Success.");
	}

	void Debug::OnUpdate()
	{
		SyncDebugLines();
	}

	void Debug::SyncDebugLines()
	{
		using namespace DirectX;

		using namespace std::chrono;

		using namespace dsr::directX;
		using namespace dsr::ecs;

		std::optional<uint32_t> currentSceneId = SceneManager->GetActiveSceneId();
		time_point<steady_clock, duration<float, std::milli>> now = steady_clock::now();
		bool erasedFromCurrentScene = false;

		std::vector<float> remainingSceneVertexData;

		for (auto it = m_lineData.begin(); it != m_lineData.end();)
		{
			if (now >= it->ExpirationDate)
			{
				erasedFromCurrentScene = erasedFromCurrentScene || (currentSceneId.has_value() && it->SceneId == currentSceneId.value());
				it = m_lineData.erase(it);
			}
			else
			{
				if (currentSceneId.has_value() && it->SceneId == currentSceneId.value())
				{
					remainingSceneVertexData.push_back(it->From.x);
					remainingSceneVertexData.push_back(it->From.y);
					remainingSceneVertexData.push_back(it->From.z);

					remainingSceneVertexData.push_back(it->Color.x);
					remainingSceneVertexData.push_back(it->Color.y);
					remainingSceneVertexData.push_back(it->Color.z);
					remainingSceneVertexData.push_back(it->Color.w);

					remainingSceneVertexData.push_back(it->To.x);
					remainingSceneVertexData.push_back(it->To.y);
					remainingSceneVertexData.push_back(it->To.z);

					remainingSceneVertexData.push_back(it->Color.x);
					remainingSceneVertexData.push_back(it->Color.y);
					remainingSceneVertexData.push_back(it->Color.z);
					remainingSceneVertexData.push_back(it->Color.w);
				}

				++it;
			}
		}

		if (erasedFromCurrentScene)
		{
			const Entity& debugEntity = m_sceneDebugEntityMap[currentSceneId.value()];

			std::shared_ptr<LineListComponent> component = SceneManager->GetComponentFrom<LineListComponent>(currentSceneId.value(), debugEntity);
			Direct3dBuffer vertexBuffer = component->GetVertexBuffer();

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			DsrResult result = Device->Map(vertexBuffer.GetBufferPtr().get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (result.GetResultStatusCode() == RESULT_SUCCESS)
			{
				memcpy(mappedResource.pData, remainingSceneVertexData.data(), sizeof(float) * remainingSceneVertexData.size());
				Device->Unmap(vertexBuffer.GetBufferPtr().get(), 0);
			}

			component->SetStartVertexLocation(0);
			component->SetVertexCount(remainingSceneVertexData.size() / 7);
		}
	}

	void Debug::UpdateDebugLines(const uint32_t& sceneId, std::shared_ptr<dsr::ecs::LineListComponent> component)
	{
		using namespace dsr::directX;

		Direct3dBuffer vertexBuffer = component->GetVertexBuffer();
		std::vector<float> vertexData;

		for (const DebugLineData& lineData : m_lineData)
		{
			if (lineData.SceneId == sceneId)
			{
				vertexData.push_back(lineData.From.x);
				vertexData.push_back(lineData.From.y);
				vertexData.push_back(lineData.From.z);

				vertexData.push_back(lineData.Color.x);
				vertexData.push_back(lineData.Color.y);
				vertexData.push_back(lineData.Color.z);
				vertexData.push_back(lineData.Color.w);

				vertexData.push_back(lineData.To.x);
				vertexData.push_back(lineData.To.y);
				vertexData.push_back(lineData.To.z);

				vertexData.push_back(lineData.Color.x);
				vertexData.push_back(lineData.Color.y);
				vertexData.push_back(lineData.Color.z);
				vertexData.push_back(lineData.Color.w);
			}
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DsrResult result = Device->Map(vertexBuffer.GetBufferPtr().get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (result.GetResultStatusCode() == RESULT_SUCCESS)
		{
			memcpy(mappedResource.pData, vertexData.data(), sizeof(float) * vertexData.size());
			Device->Unmap(vertexBuffer.GetBufferPtr().get(), 0);
		}

		component->SetStartVertexLocation(0);
		component->SetVertexCount(vertexData.size() / 7);
	}
}
