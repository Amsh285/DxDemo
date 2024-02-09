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
			OnStart = std::bind(&LineListRendererSystem::Startup, this, std::placeholders::_1);
			OnUpdate = std::bind(&LineListRendererSystem::Update, this, std::placeholders::_1);
			OnPrepareRendererUpdate = std::bind(&LineListRendererSystem::PrepareRendererUpdate, this);
		}

		void LineListRendererSystem::Startup(const EngineStartupContext& context)
		{
			std::vector<Entity> matches = context.FindEntitiesByTag("LineListShaderProgram");

			if (matches.empty())
			{
				//Todo: warn here when we have a logging framework.
				return;
			}
				
			std::shared_ptr<ShaderProgramComponent> program = context.GetComponentFrom<ShaderProgramComponent>(matches[0]);

			if (!program)
			{
				//Todo: same
				return;
			}

			m_shaderProgram = program->GetShaderProgram();
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
