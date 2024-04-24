#pragma once

#include "Component.h"
#include "Entity.h"

namespace dsr
{
	namespace ecs
	{
		class EntityComponentStore
		{
		public:
			// using ska::flat_hashmap cannot show ComponentTypeMap while debugging
			using ComponentTypeMap = std::unordered_map<std::type_index, std::shared_ptr<dsr::ecs::Component>>;
			using EntityComponentMap = std::unordered_map<dsr::ecs::Entity, ComponentTypeMap>;

			template<class TComponent>
			bool HasComponent(const Entity& entity) const
			{
				static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must derive Component.");

				return HasComponent(entity, std::type_index(typeid(TComponent)));
			}

			bool HasComponent(const Entity& entity, const std::type_index& componentType) const;

			EntityComponentMap& GetEntityComponentMap() { return m_entityComponents; }

			EntityComponentStore() = default;
			EntityComponentStore(const EntityComponentStore& other) = delete;
			EntityComponentStore& operator=(const EntityComponentStore& other) = delete;

			template<class TComponent, class ...TArgs>
			std::shared_ptr<TComponent> AddComponent(const dsr::ecs::Entity& entity, TArgs... args)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				std::shared_ptr<TComponent> component = std::make_shared<TComponent>(args...);
				AddComponent(entity, std::type_index(typeid(TComponent)), component);
				return component;
			}

			template<class TComponent>
			void AddComponent(const dsr::ecs::Entity& entity, const std::shared_ptr<TComponent>& component)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				AddComponent(entity, std::type_index(typeid(TComponent)), component);
			}

			void AddComponent(
				const dsr::ecs::Entity& entity,
				const std::type_index& componentType,
				const std::shared_ptr<dsr::ecs::Component>& component);

			template<class TComponent>
			void RemoveComponent(const dsr::ecs::Entity& entity)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				RemoveComponent(entity, std::type_index(typeid(TComponent)));
			}

			void RemoveComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType);

			void Clear();
			void Clear(const dsr::ecs::Entity& entity);
		protected:
			std::unordered_map<std::string, std::vector<Entity>> m_taggedEntities;
			EntityComponentMap m_entityComponents;
		private:
		};
	}
}



