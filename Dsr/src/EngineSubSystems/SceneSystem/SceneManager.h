#pragma once

#include "ErrorHandling/NotFoundError.h"

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/SceneSystem/Scene.h"
#include "EngineSubSystems/SceneSystem/SceneProxy.h"

namespace dsr
{
	namespace scenesystem
	{
		class SceneManager
		{
		public:
			SceneManager(const std::shared_ptr<dsr::ecs::EcsManager>& ecaManager);
			SceneManager(const SceneManager& other) = delete;
			SceneManager& operator=(const SceneManager& other) = delete;

			dsr::ecs::Entity CreateNewEntity() const;
			
			std::shared_ptr<Scene> CreateNewScene(const std::string& name);
			
			std::optional<std::shared_ptr<SceneProxy>> SetActiveScene(const std::shared_ptr<Scene>& scene);
		private:
			std::shared_ptr<dsr::ecs::EcsManager> m_ecsManager;

			std::vector<std::shared_ptr<Scene>> m_scenes;
		};
	}
}
