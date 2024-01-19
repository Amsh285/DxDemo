#pragma once

#include "EngineSubSystems/EntityComponentSystem/EngineContext.h"

namespace dsr
{
	namespace ecs
	{
		class EcsEngineContext : public EngineContext
		{
		public:
			void SetCurrentEntity(const Entity& current) { m_currentEntity = current; }
			const std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>>& GetEntityComponents() const { return m_entityComponents; }

			template<class TComponent>
			void AddComponent(const Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				if (HasComponent<TComponent>(entity))
					return;

				std::shared_ptr<Component> componentPtr = std::static_pointer_cast<Component>(component);
				m_entityComponents[entity][std::type_index(typeid(TComponent))] = componentPtr;
				
				if (std::type_index(typeid(TComponent)) == std::type_index(typeid(TagComponent)))
				{
					auto tagComponentPtr = std::static_pointer_cast<TagComponent>(componentPtr);
					std::string tag = tagComponentPtr->GetTag();
					m_taggedEntities[tag].push_back(entity);
				}
			}

			template<class TComponent>
			void RemoveComponent(const Entity& entity)
			{
				if (!HasComponent<TComponent>(entity))
					return;

				if (std::type_index(typeid(TComponent)) == std::type_index(typeid(TagComponent)))
				{
					std::shared_ptr<Component> componentPtr = m_entityComponents.at(entity).at(std::type_index(typeid(TComponent)));
					auto tagComponentPtr = std::static_pointer_cast<TagComponent>(componentPtr);
					std::string tag = tagComponentPtr->GetTag();
					 
					std::vector<Entity>& entities = m_taggedEntities.at(tag);
					auto it = std::find(entities.begin(), entities.end(), entity);
					entities.erase(it);

					m_entityComponents.at(entity).erase(std::type_index(typeid(TComponent)));
				}
				else
					m_entityComponents.at(entity).erase(std::type_index(typeid(TComponent)));
			}
		};
	}
}
