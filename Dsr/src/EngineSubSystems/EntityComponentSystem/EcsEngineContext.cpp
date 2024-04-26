#include "dsrpch.h"
#include "EcsEngineContext.h"

namespace dsr
{
	namespace ecs
	{
		void EcsEngineContext::AddComponent(const Entity& entity, const std::type_index& componentType, const std::shared_ptr<Component>& component)
		{
			m_entityComponents.AddComponent(entity, componentType, component);
		}

		void EcsEngineContext::RemoveComponent(const Entity& entity, const std::type_index& componentType)
		{
			m_entityComponents.RemoveComponent(entity, componentType);
		}
	}
}
