#include "dsrpch.h"
#include "EcsEngineContext.h"

namespace dsr
{
	namespace ecs
	{
		void EcsEngineContext::AddComponent(const Entity& entity, const std::type_index& componentType, const std::shared_ptr<Component>& component)
		{
			if (HasComponent(entity, componentType))
				return;

			m_entityComponents[entity][componentType] = component;

			if (componentType == std::type_index(typeid(TagComponent)))
			{
				auto tagComponentPtr = std::static_pointer_cast<TagComponent>(component);
				std::string tag = tagComponentPtr->GetTag();
				m_taggedEntities[tag].push_back(entity);
			}
		}

		void EcsEngineContext::RemoveComponent(const Entity& entity, const std::type_index& componentType)
		{
			if (!HasComponent(entity, componentType))
				return;

			if (componentType == std::type_index(typeid(TagComponent)))
			{
				std::shared_ptr<Component> componentPtr = m_entityComponents[entity][componentType];
				auto tagComponentPtr = std::static_pointer_cast<TagComponent>(componentPtr);
				std::string tag = tagComponentPtr->GetTag();

				std::vector<Entity>& entities = m_taggedEntities[tag];
				entities.erase(std::find(entities.begin(), entities.end(), entity));
			}

			m_entityComponents[entity].erase(componentType);
		}
	}
}
