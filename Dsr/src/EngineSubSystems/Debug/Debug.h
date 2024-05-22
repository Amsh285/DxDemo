#pragma once

#include "DirectX/Direct3dDevice.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

#include "ErrorHandling/DsrResult.h"

namespace dsr
{
	struct DebugLineData
	{
		DirectX::XMFLOAT3 From;
		DirectX::XMFLOAT3 To;
		DirectX::XMFLOAT4 Color;
		uint32_t SceneId;
		std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<float, std::milli>> ExpirationDate;
	};

	class Debug
	{
	public:
		static std::shared_ptr<dsr::directX::Direct3dDevice> Device;
		static std::shared_ptr<dsr::scene::SceneManager> SceneManager;

		static DsrResult DrawRay(
			const DirectX::XMVECTOR& rayOrigin,
			const DirectX::XMVECTOR& rayDirection,
			const DirectX::XMFLOAT4& color,
			const float& offset = 0.2f,
			const float& scale = 10.0f,
			const std::chrono::duration<float, std::milli> expiration = std::chrono::seconds(2));

		static DsrResult DrawLine(
			const DirectX::XMFLOAT3& from,
			const DirectX::XMFLOAT3& to,
			const DirectX::XMFLOAT4& color,
			const std::chrono::duration<float, std::milli>& expiration = std::chrono::seconds(2));

		static void OnUpdate();
	private:
		static std::unordered_map<uint32_t, dsr::ecs::Entity> m_sceneDebugEntityMap;
		static std::vector<DebugLineData> m_lineData;
		
		static void SyncDebugLines();
		static void UpdateDebugLines(const uint32_t& sceneId, std::shared_ptr<dsr::ecs::LineListComponent> component);
	};
}
