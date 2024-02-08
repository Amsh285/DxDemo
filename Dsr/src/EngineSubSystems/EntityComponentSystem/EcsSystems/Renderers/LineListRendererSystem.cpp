#include "dsrpch.h"
#include "LineListRendererSystem.h"

namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> LineListRendererSystem::GetRequiredComponents() const
		{
			return { std::type_index(typeid(LineListComponent)) };
		}

		LineListRendererSystem::LineListRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device)
			: RendererSystem(std::type_index(typeid(LineListRendererSystem))),
			m_device(device)
		{
			OnUpdate = std::bind(&LineListRendererSystem::Update, this, std::placeholders::_1);
			OnPrepareRendererUpdate = std::bind(&LineListRendererSystem::PrepareRendererUpdate, this);
		}

		void LineListRendererSystem::PrepareRendererUpdate()
		{
			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		void LineListRendererSystem::Update(const EngineContext& context)
		{
			std::shared_ptr<LineListComponent> lines = context.GetComponent<LineListComponent>();
		}
	}
}
