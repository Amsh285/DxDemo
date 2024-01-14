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

		RendererSystem::RendererSystem()
			: System(std::type_index(typeid(RendererSystem)), 2000500)
		{
			OnUpdate = std::bind(&RendererSystem::Update, this, std::placeholders::_1);
		}

		void RendererSystem::Update(const EngineContext& context)
		{
			//retrieve camera..
			//implement -> context.GetEntitiesWithTag("Camera"); get first
			//Use the entity to get the components
		}
	}
}
