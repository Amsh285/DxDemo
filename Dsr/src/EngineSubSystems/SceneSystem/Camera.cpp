#include "dsrpch.h"
#include "Camera.h"

#include "Infrastructure/XMathHelper.h"

namespace dsr
{
	namespace scene
	{
		std::shared_ptr<Camera> Camera::s_activeCamera;

		Camera::Camera(
			const dsr::ecs::Entity& entity,
			const uint32_t& sceneId,
			const std::shared_ptr<dsr::ecs::NameComponent>& name,
			const std::shared_ptr<dsr::ecs::TagComponent>& tag,
			const std::shared_ptr<dsr::ecs::TransformComponent>& transform,
			const std::shared_ptr<dsr::ecs::ViewFrustumComponent>& viewFrustum,
			const std::shared_ptr<dsr::ecs::ViewProjectionComponent>& viewProjection)
			: m_entity(entity), m_sceneId(sceneId),
			m_name(name), m_tag(tag),
			m_transform(transform), m_viewFrustum(viewFrustum), m_viewProjection(viewProjection)
		{
		}

		DirectX::XMVECTOR Camera::ScreenToWorld(
			const int32_t& mouseX, const int32_t& mouseY,
			const int32_t& clientWidth, const int32_t& clientHeight)
		{
			using namespace DirectX;

			XMVECTOR rayDirectionWorldSpace = dsr::ScreenToWorld(
				mouseX, mouseY,
				clientWidth, clientHeight,
				m_viewProjection->GetProjectionMatrix(), m_viewProjection->GetViewMatrix()
			);

			return XMVector4Normalize(XMVectorSubtract(rayDirectionWorldSpace, m_transform->GetPosition()));
		}
	}
}
