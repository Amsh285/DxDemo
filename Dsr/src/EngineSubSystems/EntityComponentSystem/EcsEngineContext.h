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
			EntityComponentStore::EntityComponentMap& GetEntityComponents() { return m_entityComponents.GetEntityComponentMap(); }

			template<class TComponent>
			void AddComponent(const Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				std::shared_ptr<Component> componentPtr = std::static_pointer_cast<Component>(component);
				AddComponent(entity, std::type_index(typeid(TComponent)), componentPtr);
			}

			void AddComponent(
				const Entity& entity,
				const std::type_index& componentType,
				const std::shared_ptr<Component>& component
			);

			template<class TComponent>
			void RemoveComponent(const Entity& entity)
			{ 
				RemoveComponent(entity, std::type_index(typeid(TComponent)));
			}

			void RemoveComponent(const Entity& entity, const std::type_index& componentType);
		};
	}
}
