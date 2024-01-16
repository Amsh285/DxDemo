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
			}

			template<class TComponent>
			void RemoveComponent(const Entity& entity)
			{
				if (!HasComponent<TComponent>(entity))
					return;

				m_entityComponents.at(entity).erase(std::type_index(typeid(TComponent)));
			}
		};
	}
}
