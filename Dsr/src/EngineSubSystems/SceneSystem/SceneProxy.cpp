#include "dsrpch.h"
#include "SceneProxy.h"

namespace dsr
{
	namespace scenesystem
	{
		SceneProxy::SceneProxy(const std::shared_ptr<Scene>& scene, const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager)
			: m_scene(scene), m_ecsManager(ecsManager)
		{
			assert(m_scene);
			assert(m_ecsManager);
		}

		void SceneProxy::AddComponent(const dsr::ecs::Entity& entity, const std::type_index& componentType, const std::shared_ptr<dsr::ecs::Component>& component)
		{
			m_scene->AddComponent(entity, componentType, component);
			m_ecsManager->RegisterComponent(entity, componentType, component);
		}

		void SceneProxy::LoadEntities()
		{
			const Scene::EntityComponentMap& entityMap = m_scene->GetEntityComponents();

			for (auto itEntity = entityMap.begin(); itEntity != entityMap.end(); itEntity++)
			{
				const Scene::ComponentTypeMap& componentMap = itEntity->second;

				for (auto itComponent = componentMap.begin(); itComponent != componentMap.end(); itComponent++)
				{
					m_ecsManager->RegisterComponent(itEntity->first, itComponent->first, itComponent->second);
				}
			}
		}

		void SceneProxy::UnloadEntities()
		{
			const Scene::EntityComponentMap& entityMap = m_scene->GetEntityComponents();

			for (auto itEntity = entityMap.begin(); itEntity != entityMap.end(); itEntity++)
			{
				const Scene::ComponentTypeMap& componentMap = itEntity->second;

				for (auto itComponent = componentMap.begin(); itComponent != componentMap.end(); itComponent++)
				{
					m_ecsManager->RemoveComponent(itEntity->first, itComponent->first);
				}
			}
		}

		bool SceneProxy::Equals(const uint32_t& sceneId)
		{
			return m_scene->GetSceneId() == sceneId;
		}

		bool SceneProxy::Equals(const std::shared_ptr<Scene>& scene)
		{
			return scene && m_scene->GetSceneId() == scene->GetSceneId();
		}
	}
}
