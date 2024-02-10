#include "dsrpch.h"
#include "StaticMeshRendererSystem.h"

namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> StaticMeshRendererSystem::GetRequiredComponents() const
		{
			return {
				std::type_index(typeid(TransformComponent)),
				std::type_index(typeid(StaticMeshComponent))
			};
		}

		StaticMeshRendererSystem::StaticMeshRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device)
			: RendererSystem(std::type_index(typeid(StaticMeshRendererSystem)), 2000500)
		{
			OnStart = std::bind(&StaticMeshRendererSystem::Startup, this, std::placeholders::_1);
			OnUpdate = std::bind(&StaticMeshRendererSystem::Update, this, std::placeholders::_1);
			OnPrepareRendererUpdate = std::bind(&StaticMeshRendererSystem::PrepareRendererUpdate, this, std::placeholders::_1);
			m_device = device;
		}

		DsrResult StaticMeshRendererSystem::Initialize()
		{
			using namespace directX;

			std::variant<Direct3dSamplerState, dsr_error> createSamplerStateResult = Direct3dSamplerState::CreateDefault(m_device);
			if (std::holds_alternative<dsr_error>(createSamplerStateResult))
			{
				const dsr_error& error = std::get<dsr_error>(createSamplerStateResult);
				std::string errorMessage = "Error initializing Renderer: ";
				errorMessage += error.what();
				return DsrResult(errorMessage, error.GetResult());
			}

			m_defaultSamplerState = std::get<Direct3dSamplerState>(createSamplerStateResult);
			SetDefaultSamplerState();

			return DsrResult::Success("Initializing Renderer Successful.");
		}

		void StaticMeshRendererSystem::SetDefaultSamplerState()
		{
			ID3D11SamplerState* state = m_defaultSamplerState.GetSamplerStatePtr().get();
			m_device->SetSamplers<ID3D11PixelShader>(0, 1, &state);
		}

		void StaticMeshRendererSystem::Startup(const EngineStartupContext& context)
		{
		}

		void StaticMeshRendererSystem::PrepareRendererUpdate(const EnginePrepareRendererContext& context)
		{
			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			SetDefaultSamplerState();
			m_device->SetDefaultRasterizerState();
		}

		void StaticMeshRendererSystem::Update(const EngineContext& context)
		{
			using namespace dsr::directX;

			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<StaticMeshComponent> staticMesh = context.GetComponent<StaticMeshComponent>();

			std::vector<Entity> cameras = context.FindEntitiesByTag("Camera");
			std::vector<Entity> defaultShaderProgramEntities = context.FindEntitiesByTag("DefaultShaderProgram");

			if (cameras.size() < 1 || defaultShaderProgramEntities.size() < 1)
				return;

			SetupMvp(context, cameras[0], transform->GetRenderTransform());

			std::shared_ptr<ShaderProgramComponent> defaultShaderProgramComponent = context.GetComponentFrom<ShaderProgramComponent>(defaultShaderProgramEntities[0]);
			std::shared_ptr<Direct3dShaderProgram> defaultShaderProgram = defaultShaderProgramComponent->GetShaderProgram();

			m_device->SetInputLayout(defaultShaderProgram->VertexShaderInputLayout.get());

			Direct3dVertexBufferf vertexBuffer = staticMesh->GetVertexBuffer();
			Direct3dShaderInputLayout layout = vertexBuffer.GetLayout();
			uint32_t vertexStride = layout.GetTotalStride();
			uint32_t offset = 0;

			std::shared_ptr<ID3D11Buffer> vertexBufferPtr = vertexBuffer.GetVertexBuffer().GetBufferPtr();
			ID3D11Buffer* vertexShaderRawPtr = vertexBufferPtr.get();

			m_device->SetVertexBuffers(0, 1, &vertexShaderRawPtr, &vertexStride, &offset);

			std::shared_ptr<ID3D11Buffer> indexBufferPtr = vertexBuffer.GetIndexBuffer().GetBufferPtr();
			m_device->SetIndexBuffer(indexBufferPtr.get());

			m_device->UseShader(defaultShaderProgram->VertexShader->GetShaderPtr().get(), nullptr, 0);

			for (std::shared_ptr<rendering::VertexGroup> vertexGroup : staticMesh->GetVertexGroups())
			{
				if (vertexGroup->PixelShader)
					m_device->UseShader(vertexGroup->PixelShader->GetShaderPtr().get(), nullptr, 0);
				else
					m_device->UseShader(defaultShaderProgram->PixelShader->GetShaderPtr().get(), nullptr, 0);

				std::shared_ptr<TransformComponent> cameraTransform = context.GetComponentFrom<TransformComponent>(cameras[0]);

				DirectX::XMStoreFloat4(&vertexGroup->PSData.CameraPosition, cameraTransform->GetPosition());
				SetConstantBuffer(m_device, m_psConstantBuffers, 0, &vertexGroup->PSData, sizeof(PixelShaderData));

				ApplyConstantBuffers<ID3D11PixelShader>();
				SetupTextures(vertexGroup);
				m_device->DrawIndexed(vertexGroup->IndexCount, vertexGroup->StartIndexLocation, 0);
			}
		}

		void StaticMeshRendererSystem::SetupMvp(const EngineContext& context, const Entity& camera, const RenderTransform& renderTransform)
		{
			std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponentFrom<ViewProjectionComponent>(camera);

			SetConstantBuffer(m_device, m_vsConstantBuffers, 0, viewProjection->GetProjectionMatrix());
			SetConstantBuffer(m_device, m_vsConstantBuffers, 1, viewProjection->GetViewMatrix());
			SetConstantBuffer(m_device, m_vsConstantBuffers, 2, &renderTransform, sizeof(RenderTransform));
			ApplyConstantBuffers<ID3D11VertexShader>();
		}

		void StaticMeshRendererSystem::SetupTextures(std::shared_ptr<dsr::directX::rendering::VertexGroup> vertexGroup)
		{
			using namespace dsr::directX;

			std::vector<ID3D11ShaderResourceView*> psResourceViews;
			for (const Direct3dShaderTexture2D& texture : vertexGroup->PSTextures2D)
				psResourceViews.push_back(texture.GetShaderResourceViewPtr().get());

			m_device->SetShaderResources<ID3D11PixelShader>(0, psResourceViews.size(), psResourceViews.data());
		}
	}
}
