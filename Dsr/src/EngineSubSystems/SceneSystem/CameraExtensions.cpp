#include "dsrpch.h"
#include "CameraExtensions.h"

namespace dsr
{
	namespace scene
	{
		std::shared_ptr<Camera> CreateGlobalCamera(const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager)
		{
			using namespace dsr::ecs;

			const dsr::ecs::Entity cameraEntity = ecsManager->CreateNewEntity();

			std::shared_ptr<NameComponent> name = ecsManager->RegisterComponent<NameComponent>(cameraEntity, "Default-Camera");
			std::shared_ptr<TagComponent> tag = ecsManager->RegisterComponent<TagComponent>(cameraEntity, DefaultCameraTag);

			std::shared_ptr<TransformComponent> transform = ecsManager->RegisterComponent<TransformComponent>(cameraEntity);
			std::shared_ptr<ViewFrustumComponent> viewFrustum = ecsManager->RegisterComponent<ViewFrustumComponent>(cameraEntity);
			std::shared_ptr<ViewProjectionComponent> viewProjection = ecsManager->RegisterComponent<ViewProjectionComponent>(cameraEntity);

			return std::make_shared<Camera>(
				cameraEntity, 0,
				name, tag,
				transform, viewFrustum, viewProjection
			);
		}

		std::shared_ptr<Camera> CreateCameraForActiveScene(const std::shared_ptr<SceneManager>& sceneManager)
		{
			using namespace dsr::ecs;

			const dsr::ecs::Entity cameraEntity = sceneManager->CreateNewEntity();
			const std::optional<uint32_t> activeSceneId = sceneManager->GetActiveSceneId();

			if (!activeSceneId.has_value())
				return nullptr;

			std::shared_ptr<NameComponent> name = sceneManager->AddComponent<NameComponent>(activeSceneId.value(), cameraEntity, "Default-Camera");
			std::shared_ptr<TagComponent> tag = sceneManager->AddComponent<TagComponent>(activeSceneId.value(), cameraEntity, DefaultCameraTag);

			std::shared_ptr<TransformComponent> transform = sceneManager->AddComponent<TransformComponent>(activeSceneId.value(), cameraEntity);
			std::shared_ptr<ViewFrustumComponent> viewFrustum = sceneManager->AddComponent<ViewFrustumComponent>(activeSceneId.value(), cameraEntity);
			std::shared_ptr<ViewProjectionComponent> viewProjection = sceneManager->AddComponent<ViewProjectionComponent>(activeSceneId.value(), cameraEntity);

			return std::make_shared<Camera>(
				cameraEntity, activeSceneId.value(),
				name, tag,
				transform, viewFrustum, viewProjection
			);
		}
	}
}
