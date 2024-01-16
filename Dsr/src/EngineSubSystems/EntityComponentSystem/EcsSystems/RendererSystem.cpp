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

		void RendererSystem::PrepareUpdate(const events::PrepareUdateFrameEvent& args)
		{
			m_device->Clear(0.0f, 0.2f, 0.4f, 1.0f);
			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void RendererSystem::Update(const EngineContext& context)
		{
			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<StaticMeshComponent> staticMesh = context.GetComponent< StaticMeshComponent>();


		}

		void RendererSystem::UpdateFinished(const events::UpdateFrameFinishedEvent& args)
		{
			m_device->SwapBuffers();
		}
	}
}
