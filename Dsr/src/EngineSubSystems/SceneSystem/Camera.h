#pragma once

#include "EngineSubSystems/EntityComponentSystem/Entity.h"

#include "EngineSubSystems/EntityComponentSystem/Components/NameComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TagComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewFrustumComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ViewProjectionComponent.h"

namespace dsr
{
	namespace scene
	{
		constexpr const char DefaultCameraTag[] = "Camera";

		struct PixelShaderCameraData
		{
			DirectX::XMFLOAT4 CameraPosition;

			PixelShaderCameraData()
				: CameraPosition(0.0f, 0.0f, 0.0f, 1.0f)
			{
			}
		};

		class Camera final
		{
		public:
			dsr::ecs::Entity GetEntity() const { return m_entity; }
			uint32_t GetSceneId() const { return m_sceneId; }

			std::shared_ptr<dsr::ecs::NameComponent> GetName() const { return m_name; }
			std::shared_ptr<dsr::ecs::TagComponent> GetTag() const { return m_tag; }

			std::shared_ptr<dsr::ecs::TransformComponent> GetTransform() const { return m_transform; }
			std::shared_ptr<dsr::ecs::ViewFrustumComponent> GetViewFrustum() const { return m_viewFrustum; }
			std::shared_ptr<dsr::ecs::ViewProjectionComponent> GetViewProjection() const { return m_viewProjection; }
			
			static std::shared_ptr<Camera> GetActiveCamera() { return s_activeCamera; }
			static void SetActiveCamera(const std::shared_ptr<Camera>& camera) { s_activeCamera = camera; };

			Camera(
				const dsr::ecs::Entity& entity,
				const uint32_t& sceneId,
				const std::shared_ptr<dsr::ecs::NameComponent>& name,
				const std::shared_ptr<dsr::ecs::TagComponent>& tag,
				const std::shared_ptr<dsr::ecs::TransformComponent>& transform,
				const std::shared_ptr<dsr::ecs::ViewFrustumComponent>& viewFrustum,
				const std::shared_ptr<dsr::ecs::ViewProjectionComponent>& viewProjection);

			DirectX::XMVECTOR ScreenToWorld(
				const int32_t& mouseX, const int32_t& mouseY,
				const int32_t& clientWidth, const int32_t& clientHeight);
		private:
			dsr::ecs::Entity m_entity;
			uint32_t m_sceneId;

			std::shared_ptr<dsr::ecs::NameComponent> m_name;
			std::shared_ptr<dsr::ecs::TagComponent> m_tag;

			std::shared_ptr<dsr::ecs::TransformComponent> m_transform;
			std::shared_ptr<dsr::ecs::ViewFrustumComponent> m_viewFrustum;
			std::shared_ptr<dsr::ecs::ViewProjectionComponent> m_viewProjection;

			static std::shared_ptr<Camera> s_activeCamera;
		};
	}
}
