#pragma once

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/EcsManager.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewFrustumComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewProjectionComponent.h"

#include "EngineSubSystems/SceneSystem/SceneManager.h"

namespace dsr
{
	namespace scene
	{
		class Camera final
		{
		public:
			dsr::ecs::Entity GetEntity() const { return m_entity; }
			uint32_t GetSceneId() const { return m_sceneId; }

			std::shared_ptr<dsr::ecs::TransformComponent> GetTransform() const { return m_transform; }
			std::shared_ptr<dsr::ecs::ViewFrustumComponent> GetViewFrustum() const { return m_viewFrustum; }
			std::shared_ptr<dsr::ecs::ViewProjectionComponent> GetViewProjection() const { return m_viewProjection; }

			static std::shared_ptr<Camera> CreateGlobal(const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager);
			static std::shared_ptr<Camera> CreateForActiveScene(const std::shared_ptr<SceneManager>& sceneManager);
			
			static std::shared_ptr<Camera> GetActiveCamera() { return s_activeCamera; }
			static void SetActiveCamera(const std::shared_ptr<Camera>& camera) { s_activeCamera = camera; };

			Camera(
				const dsr::ecs::Entity& entity,
				const uint32_t& sceneId,
				const std::shared_ptr<dsr::ecs::TransformComponent>& transform,
				const std::shared_ptr<dsr::ecs::ViewFrustumComponent>& viewFrustum,
				const std::shared_ptr<dsr::ecs::ViewProjectionComponent>& viewProjection);
		private:
			dsr::ecs::Entity m_entity;
			uint32_t m_sceneId;

			std::shared_ptr<dsr::ecs::TransformComponent> m_transform;
			std::shared_ptr<dsr::ecs::ViewFrustumComponent> m_viewFrustum;
			std::shared_ptr<dsr::ecs::ViewProjectionComponent> m_viewProjection;

			static std::shared_ptr<Camera> s_activeCamera;
		};
	}
}
