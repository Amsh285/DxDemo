#include "dsrpch.h"
#include "Scene.h"

namespace dsr
{
	namespace scenesystem
	{
		std::set<dsr::ecs::Entity> Scene::GetEntities() const
		{
			std::set<dsr::ecs::Entity> result;

			for (auto it = m_entityComponents.begin(); it != m_entityComponents.end(); it++)
			{
				result.insert(it->first);
			}

			return result;
		}

		Scene::Scene()
		{
			static std::atomic<uint32_t> uid = 0;

			m_id = uid.fetch_add(1);
		}

		Scene::Scene(const std::string& name)
			: Scene()
		{
			m_name = name;
		}

		void Scene::AddComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType, const std::shared_ptr<dsr::ecs::Component>& component)
		{
			ComponentTypeMap& entityComponents = m_entityComponents[entity];

			auto it = entityComponents.find(componentType);

			if (it != entityComponents.end())
				return;

			entityComponents[componentType] = component;
		}

		void Scene::RemoveComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType)
		{
			auto it = m_entityComponents.find(entity);

			if (it == m_entityComponents.end())
				return;

			it->second.erase(componentType);
		}
	}
}
