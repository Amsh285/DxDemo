#include "dsrpch.h"
#include "EngineContext.h"


namespace dsr
{
	namespace ecs
	{
		bool EngineContext::Exists(const Entity& entity) const
		{
			return m_entityComponents.find(entity) != m_entityComponents.end();
		}

		bool EngineContext::HasComponent(const Entity& entity, const std::type_index& componentType) const
		{
			if (!Exists(entity))
				return false;

			return m_entityComponents.at(entity).find(componentType) != m_entityComponents.at(entity).end();
		}

		std::vector<Entity> EngineContext::FindEntitiesByTag(const std::string& tag) const
		{
			auto it = m_taggedEntities.find(tag);

			if (it == m_taggedEntities.end())
				return std::vector<Entity>();

			return it->second;
		}

		std::unordered_map<std::type_index, std::shared_ptr<Component>>& EngineContext::GetComponents(const Entity& entity)
		{
			return m_entityComponents[entity];
		}
	}
}
