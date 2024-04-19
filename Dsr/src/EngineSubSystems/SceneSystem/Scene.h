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

			std::string GetName() const { return m_name; }
			void SetName(const std::string& name) { m_name = name; }

			std::set<dsr::ecs::Entity> GetEntities() const;

			Scene();
			Scene(const Scene& other) = delete;
			Scene& operator=(const Scene& other) = delete;

			template<class TComponent, class ...TArgs>
			void AddComponent(const dsr::ecs::Entity& entity, TArgs... args)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				ComponentTypeMap& entityComponents = m_entityComponents[entity];
				std::type_index typeKey = std::type_index(typeid(TComponent));

				auto it = entityComponents.find(typeKey);

				if (it != entityComponents.end())
					throw dsr::InvalidOperationError("cannot add duplicate Component of type: " + typeKey);

				std::shared_ptr<TComponent> component = std::make_shared<TComponent>(args...);
				entityComponents[typeKey] = component;
			}

			template<class TComponent>
			void AddComponent(const std::shared_ptr<TComponent>& component, const dsr::ecs::Entity& entity)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				ComponentTypeMap& entityComponents = m_entityComponents[entity];
				std::type_index typeKey = std::type_index(typeid(TComponent));

				auto it = entityComponents.find(typeKey);

				if (it != entityComponents.end())
					throw dsr::InvalidOperationError("cannot add duplicate Component of type: " + typeKey);

				entityComponents[typeKey] = component;
			}

			template<class TComponent>
			void RemoveComponent(const dsr::ecs::Entity& entity)
			{
				static_assert(std::is_base_of<dsr::ecs::Component, TComponent>::value, "TComponent must be derived from Component.");

				auto it = m_entityComponents.find(entity);

				if (it == m_entityComponents.end())
					return;

				it->second.erase(std::type_index(typeid(TComponent)));
			}
		private:
			uint32_t m_id;
			std::string m_name;
			EntityComponentMap m_entityComponents;
		};
	}
}
