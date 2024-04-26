#include "dsrpch.h"
#include "EngineContext.h"


namespace dsr
{
	namespace ecs
	{
		bool EngineContext::HasComponent(const Entity& entity, const std::type_index& componentType) const
		{
			return m_entityComponents.HasComponent(entity, componentType);
		}

		std::vector<Entity> EngineContext::FindEntitiesByTag(const std::string& tag) const
		{
			return m_entityComponents.FindEntitiesByTag(tag);
		}

		EntityComponentStore::ComponentTypeMap& EngineContext::GetComponents(const Entity& entity)
		{
			return m_entityComponents.GetComponentTypeMap(entity);
		}
	}
}
