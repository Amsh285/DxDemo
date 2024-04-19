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

		std::shared_ptr<Scene> SceneManager::CreateNewScene(const std::string& name)
		{
			std::shared_ptr<Scene> scene = std::make_shared<Scene>(name);
			m_scenes.push_back(scene);

			return scene;
		}

		std::optional<std::shared_ptr<SceneProxy>> SceneManager::SetActiveScene(const std::shared_ptr<Scene>& scene)
		{
			if (!scene)
				return std::nullopt;

			auto it = std::find_if(
				m_scenes.begin(),
				m_scenes.end(),
				[&scene](const std::shared_ptr<Scene>& current) {
					return current->GetSceneId() == scene->GetSceneId();
				});

			if (it == m_scenes.end())
				return std::nullopt;

			return std::make_shared<SceneProxy>(*it, m_ecsManager);
		}
	}
}
