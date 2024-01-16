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

		std::vector<Entity> EngineContext::FindEntitiesByTag(const std::string& tag) const
		{
			std::vector<Entity> matches;

			for (auto pair : m_entityComponents)
			{
				std::shared_ptr<TagComponent> tagComponent = GetComponentFrom<TagComponent>(pair.first);
				
				if (tagComponent && tagComponent->GetTag() == tag)
					matches.push_back(pair.first);
			}

			return matches;
		}

		std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> EngineContext::GetComponents(const Entity& entity) const
		{
			if (m_entityComponents.find(entity) == m_entityComponents.end())
				return std::nullopt;

			return m_entityComponents.at(entity);
		}
	}
}
