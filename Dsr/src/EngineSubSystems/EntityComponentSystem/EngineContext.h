#pragma once

#include "Components/TagComponent.h"
#include "Components/ViewProjectionComponent.h"

#include "DirectX/Direct3dDeviceShaderExtensions.h"

#include "EngineSubSystems/EntityComponentSystem/Component.h"
#include "EngineSubSystems/EntityComponentSystem/Entity.h"

namespace dsr
{
	namespace ecs
	{
		class EngineContext
		{
		public:
			Entity GetCurrentEntity() const { return m_currentEntity; }

			bool Exists(const Entity& entity) const;

			template<class TComponent>
			bool HasComponent() const
			{
				return HasComponent<TComponent>(m_currentEntity);
			}

			template<class TComponent>
			bool HasComponent(const Entity& entity) const
			{
				static_assert(std::is_base_of<Component, TComponent>::value, "TComponent must derive Component.");

				if (!Exists(entity))
					return false;

				return m_entityComponents.at(entity).find(std::type_index(typeid(TComponent))) != m_entityComponents.at(entity).end();
			}

			std::vector<Entity> FindEntitiesByTag(const std::string& tag) const;

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponent() const
			{
				return GetComponentFrom<TComponent>(m_currentEntity);
			}

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponentFrom(const Entity& entity) const
			{
				std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> componentSearchResult = GetComponents(entity);

				if (!componentSearchResult.has_value())
					return nullptr;

				std::unordered_map<std::type_index, std::shared_ptr<Component>> componentMap = componentSearchResult.value();

				auto it = componentMap.find(std::type_index(typeid(TComponent)));

				if (it == componentMap.end())
					return nullptr;

				return std::dynamic_pointer_cast<TComponent>(it->second);
			}

			std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> GetComponents(const Entity& entity) const;
		protected:
			Entity m_currentEntity = 0;
			std::unordered_map<std::string, std::vector<Entity>> m_taggedEntities;
			std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>> m_entityComponents;
		private:
		};
	}
}



