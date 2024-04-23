#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"
#include "EngineSubSystems/EntityComponentSystem/Entity.h"

#include "ErrorHandling/InvalidOperationError.h"

namespace dsr
{
	namespace scenesystem
	{
		class Scene
		{
		public:
			using ComponentTypeMap = ska::flat_hash_map<std::type_index, std::shared_ptr<dsr::ecs::Component>>;
			using EntityComponentMap = ska::flat_hash_map<dsr::ecs::Entity, ComponentTypeMap>;

			uint32_t GetSceneId() const { return m_id; }

			const EntityComponentMap& GetEntityComponents() const { return m_entityComponents; }

			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			std::set<dsr::ecs::Entity> GetEntities() const;

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponentFrom(const dsr::ecs::Entity& entity) const
			{ 
				return std::dynamic_pointer_cast<TComponent>(m_entityComponents.at(entity).at(std::type_index(typeid(TComponent))));
			}

			Scene();
			Scene(const std::string& name);
			Scene(const Scene& other) = delete;
			Scene& operator=(const Scene& other) = delete;

			template<class TComponent, class ...TArgs>
			std::shared_ptr<TComponent> AddComponent(const dsr::ecs::Entity& entity, TArgs... args)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				ComponentTypeMap& entityComponents = m_entityComponents[entity];
				std::type_index typeKey = std::type_index(typeid(TComponent));

				auto it = entityComponents.find(typeKey);

				if (it != entityComponents.end())
				{
					std::string errorMesssage = "Entity: ";
					errorMesssage += std::to_string(entity) + " ComponentType: " ;
					errorMesssage += typeKey.name();
					errorMesssage += " cannot be registered twice.";
					throw InvalidOperationError(errorMesssage);
				}

				std::shared_ptr<TComponent> component = std::make_shared<TComponent>(args...);
				entityComponents[typeKey] = component;
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
		private:
			uint32_t m_id;
			std::string m_name;
			EntityComponentMap m_entityComponents;
		};
	}
}
