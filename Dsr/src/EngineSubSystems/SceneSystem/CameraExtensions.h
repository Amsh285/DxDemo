#pragma once

#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"

#include "EngineSubSystems/SceneSystem/Camera.h"
#include "EngineSubSystems/SceneSystem/SceneManager.h"

namespace dsr
{
	namespace scene
	{
		std::shared_ptr<Camera> CreateGlobalCamera(const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager);
		std::shared_ptr<Camera> CreateCameraForActiveScene(const std::shared_ptr<SceneManager>& sceneManager);
	}
}
