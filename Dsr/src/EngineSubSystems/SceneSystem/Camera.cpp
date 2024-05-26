#include "dsrpch.h"
#include "Camera.h"

namespace dsr
{
	namespace scene
	{
		std::shared_ptr<Camera> Camera::s_activeCamera;

		std::shared_ptr<Camera> Camera::CreateGlobal(const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager)
		{
			using namespace dsr::ecs;

			const dsr::ecs::Entity cameraEntity = ecsManager->CreateNewEntity();

			std::shared_ptr<TransformComponent> transform = ecsManager->RegisterComponent<TransformComponent>(cameraEntity);
			std::shared_ptr<ViewFrustumComponent> viewFrustum = ecsManager->RegisterComponent<ViewFrustumComponent>(cameraEntity);
			std::shared_ptr<ViewProjectionComponent> viewProjection = ecsManager->RegisterComponent<ViewProjectionComponent>(cameraEntity);

			return std::make_shared<Camera>(cameraEntity, 0, transform, viewFrustum, viewProjection);
		}

		std::shared_ptr<Camera> Camera::CreateForActiveScene(const std::shared_ptr<SceneManager>& sceneManager)
		{
			using namespace dsr::ecs;

			const dsr::ecs::Entity cameraEntity = sceneManager->CreateNewEntity();
			const std::optional<uint32_t> activeSceneId = sceneManager->GetActiveSceneId();

			if (!activeSceneId.has_value())
				return nullptr;

			std::shared_ptr<TransformComponent> transform = sceneManager->AddComponent<TransformComponent>(activeSceneId.value(), cameraEntity);
			std::shared_ptr<ViewFrustumComponent> viewFrustum = sceneManager->AddComponent<ViewFrustumComponent>(activeSceneId.value(), cameraEntity);
			std::shared_ptr<ViewProjectionComponent> viewProjection = sceneManager->AddComponent<ViewProjectionComponent>(activeSceneId.value(), cameraEntity);

			return std::make_shared<Camera>(cameraEntity, activeSceneId.value(), transform, viewFrustum, viewProjection);
		}

		Camera::Camera(
			const dsr::ecs::Entity& entity,
			const uint32_t& sceneId,
			const std::shared_ptr<dsr::ecs::TransformComponent>& transform,
			const std::shared_ptr<dsr::ecs::ViewFrustumComponent>& viewFrustum,
			const std::shared_ptr<dsr::ecs::ViewProjectionComponent>& viewProjection)
			: m_entity(entity), m_sceneId(sceneId),
			m_transform(transform), m_viewFrustum(viewFrustum), m_viewProjection(viewProjection)
		{
		}
	}
}
