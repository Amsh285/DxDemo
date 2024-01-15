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
			const std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>>& GetEntities() const { return m_entities; }

			template<class TComponent>
			void AddComponent(const Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				m_entities[entity][std::type_index(typeid(TComponent))] = component;
			}

			template<class TComponent>
			void RemoveComponent(const Entity& entity)
			{
				if (!HasComponent<TComponent>(entity))
					return;

				m_entities.at(entity).erase(std::type_index(typeid(TComponent)));
			}
		};
	}
}
