#include "dsrpch.h"
#include "SceneManager.h"

namespace dsr
{
	namespace scenesystem
	{
		SceneManager::SceneManager(const std::shared_ptr<dsr::ecs::EcsManager>& ecaManager)
			: m_ecsManager(ecaManager)
		{
		}

		dsr::ecs::Entity SceneManager::CreateNewEntity() const
		{
			return dsr::ecs::EcsManager::CreateNewEntity();
		}

		std::pair<uint32_t, std::string> SceneManager::CreateNewScene(const std::string& name)
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>(name);
			m_scenes.insert({ scene->GetSceneId(), scene });

			return std::pair<uint32_t, std::string>(scene->GetSceneId(), name);
		}

		void SceneManager::AddScene(const std::shared_ptr<Scene>& scene)
		{
			m_scenes.insert({ scene->GetSceneId(), scene });
		}

		void SceneManager::RemoveScene(const uint32_t& sceneId)
		{
			RemoveScene(m_scenes.at(sceneId));
		}

		void SceneManager::RemoveScene(const std::shared_ptr<Scene>& scene)
		{
			if (m_activeScene && m_activeScene->Equals(scene))
			{
				m_activeScene->UnloadEntities();
				m_activeScene.reset();
			}

			m_scenes.erase(scene->GetSceneId());
		}

		void SceneManager::AddComponent(const uint32_t& sceneId, const dsr::ecs::Entity& entity, const std::type_index& componentType, const std::shared_ptr<dsr::ecs::Component>& component)
		{
			if (m_activeScene && m_activeScene->Equals(sceneId))
			{
				m_activeScene->AddComponent(entity, componentType, component);
			}
			else
			{
				auto it = m_scenes.find(sceneId);

				if (it == m_scenes.end())
					return;

				it->second->AddComponent(entity, componentType, component);
			}
		}

		void SceneManager::SetActiveScene(const uint32_t& sceneId)
		{
			if (m_scenes.count(sceneId) == 0)
				return;

			if (m_activeScene)
			{
				if (m_activeScene->Equals(sceneId))
					return;

				m_activeScene->UnloadEntities();
			}

			m_activeScene = std::make_unique<SceneProxy>(m_scenes[sceneId], m_ecsManager);
			m_activeScene->LoadEntities();
		}
	}
}
