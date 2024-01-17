#include "dsrpch.h"
#include "RendererSystem.h"


namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> RendererSystem::GetRequiredComponents() const
		{
			return {
				std::type_index(typeid(TransformComponent)),
				std::type_index(typeid(StaticMeshComponent))
			};
		}

		RendererSystem::RendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device)
			: System(std::type_index(typeid(RendererSystem)), 2000500)
		{
			OnUpdate = std::bind(&RendererSystem::Update, this, std::placeholders::_1);

			m_device = device;
		}

		DsrResult RendererSystem::Initialize()
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

		void RendererSystem::SetDefaultSamplerState()
		{
			ID3D11SamplerState* state = m_defaultSamplerState.GetSamplerStatePtr().get();
			m_device->SetSamplers<ID3D11PixelShader>(0, 1, &state);
		}

		void RendererSystem::PrepareUpdate(const events::PrepareUdateFrameEvent& args)
		{
			m_device->Clear(0.0f, 0.2f, 0.4f, 1.0f);
			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void RendererSystem::Update(const EngineContext& context)
		{
			using namespace directX;

			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<StaticMeshComponent> staticMesh = context.GetComponent<StaticMeshComponent>();

			// i got a idea. instead of linear searching entities. Store them in a tagged map. reduce complexity to O(1)
			// then the cameralookup can stay here!
			std::vector<Entity> cameras = context.FindEntitiesByTag("Camera");

			if (cameras.size() > 0)
			{
				Entity cameraEntity = cameras[0];
				std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponentFrom<ViewProjectionComponent>(cameraEntity);

				SetConstantBuffer(m_device, m_vsConstantBuffers, 0, viewProjection->GetProjectionMatrix());
				SetConstantBuffer(m_device, m_vsConstantBuffers, 1, viewProjection->GetViewMatrix());
			}
			else
			{
				SetConstantBuffer(m_device, m_vsConstantBuffers, 0, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 1.0f, 0.1f, 1000.0f));
				SetConstantBuffer(m_device, m_vsConstantBuffers, 1, DirectX::XMMatrixIdentity());
			}

			RenderTransform rt = transform->GetRenderTransform();
			SetConstantBuffer(m_device, m_vsConstantBuffers, 2, &rt, sizeof(RenderTransform));



			/*for(auto& : )*/

		}

		void RendererSystem::UpdateFinished(const events::UpdateFrameFinishedEvent& args)
		{
			m_device->SwapBuffers();
		}
	}
}
