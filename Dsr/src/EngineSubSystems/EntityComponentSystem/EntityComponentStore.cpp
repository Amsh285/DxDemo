#include "dsrpch.h"
#include "EntityComponentStore.h"

namespace dsr
{
	namespace ecs
	{
		bool EntityComponentStore::HasComponent(const Entity& entity, const std::type_index& componentType) const
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return false;

			return it->second.find(componentType) != it->second.end();
		}

		std::vector<Entity> EntityComponentStore::FindEntitiesByTag(const std::string& tag) const
		{
			auto it = m_taggedEntities.find(tag);

			if (it == m_taggedEntities.end())
				return std::vector<Entity>();

			return std::vector<Entity>(it->second.begin(), it->second.end());
		}

		void EntityComponentStore::AddComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType, const std::shared_ptr<dsr::ecs::Component>& component)
		{
			m_entityComponents[entity].insert(std::pair(componentType, component));

			if (componentType == std::type_index(typeid(TagComponent)))
			{
				auto tagComponentPtr = std::static_pointer_cast<TagComponent>(component);
				std::string tag = tagComponentPtr->GetTag();
				m_taggedEntities[tag].insert(entity);
			}
		}

		void EntityComponentStore::RemoveComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType)
		{
			if (componentType == std::type_index(typeid(TagComponent)))
			{
				std::shared_ptr<Component> componentPtr = m_entityComponents[entity][componentType];

				if (componentPtr)
				{
					auto tagComponentPtr = std::static_pointer_cast<TagComponent>(componentPtr);
					std::set<Entity>& entities = m_taggedEntities[tagComponentPtr->GetTag()];
					entities.erase(entity);
				}
			}

			// dont search for entity. if it doenst exists just create it and let erase fail.
			// again this is unlikely to happen and most likely due to a bug
			m_entityComponents[entity].erase(componentType);
		}

		void EntityComponentStore::Clear()
		{
			m_entityComponents.clear();
			m_taggedEntities.clear();
		}

		void EntityComponentStore::Clear(const dsr::ecs::Entity& entity)
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return;

			std::shared_ptr<TagComponent> tagComponent = GetComponentFrom<TagComponent>(entity);

			if (tagComponent)
				m_taggedEntities[tagComponent->GetTag()].erase(entity);

			it->second.clear();
			m_entityComponents.erase(entity);
		}
	}
}

