#include "dsrpch.h"
#include "EntityComponentStore.h"

namespace dsr
{
	namespace ecs
	{
		bool EntityComponentStore::HasComponent(const Entity& entity, const std::type_index& componentType) const
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return false;

			return it->second.find(componentType) != it->second.end();
		}

		void EntityComponentStore::AddComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType, const std::shared_ptr<dsr::ecs::Component>& component)
		{
			m_entityComponents[entity].insert(std::pair(componentType, component));
		}

		void EntityComponentStore::RemoveComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType)
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return;

			it->second.erase(componentType);
		}

		void EntityComponentStore::Clear()
		{
			m_entityComponents.clear();
		}

		void EntityComponentStore::Clear(const dsr::ecs::Entity& entity)
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return;

			it->second.clear();
			m_entityComponents.erase(entity);
		}
	}
}

