#pragma once


#include "Systems/EntityComponentSystem/Component.h"
#include "Systems/EntityComponentSystem/Entity.h"

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

				return m_entities.at(entity).find(std::type_index(typeid(TComponent))) != m_entities.at(entity).end();
			}

			template<class TComponent>
			std::shared_ptr<TComponent> GetComponent() const
			{
				std::optional<std::unordered_map<std::type_index, std::shared_ptr<Component>>> componentSearchResult = GetComponents(m_currentEntity);

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
			std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<Component>>> m_entities;
		private:
		};
	}
}



