#include "dsrpch.h"
#include "ViewProjectionSystem.h"

namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> ViewProjectionSystem::GetRequiredComponents() const
		{
			return {
				std::type_index(typeid(TransformComponent)),
				std::type_index(typeid(ViewFrustumComponent)),
				std::type_index(typeid(ViewProjectionComponent))
			};
		}

		ViewProjectionSystem::ViewProjectionSystem()
			: System(std::type_index(typeid(ViewProjectionSystem)), 2000100), m_aspecRatio(1.0f)
		{
			OnUpdate = std::bind(&ViewProjectionSystem::Update, this, std::placeholders::_1);
		}

		void ViewProjectionSystem::HandleWindowResized(const events::WindowResizedEvent& resizedEvent)
		{
			m_aspecRatio = static_cast<float>(resizedEvent.GetClientWidth()) / static_cast<float>(resizedEvent.GetClientHeight());
		}

		void ViewProjectionSystem::Update(const EngineContext& context)
		{
			using namespace DirectX;

			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<ViewFrustumComponent> viewFrustum = context.GetComponent<ViewFrustumComponent>();
			std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponent<ViewProjectionComponent>();

			viewProjection->SetViewMatrix(GetViewMatrix(transform));
			viewProjection->SetProjectionMatrix(GetProjectionMatrix(viewFrustum));
		}

		DirectX::XMMATRIX ViewProjectionSystem::GetProjectionMatrix(const std::shared_ptr<ViewFrustumComponent>& viewFrustum)
		{
			return DirectX::XMMatrixPerspectiveFovLH(
				viewFrustum->GetFieldOfViewY(),
				m_aspecRatio,
				viewFrustum->GetNearZ(),
				viewFrustum->GetFarZ()
			);
		}

		DirectX::XMMATRIX ViewProjectionSystem::GetViewMatrix(const std::shared_ptr<TransformComponent>& transform)
		{
			using namespace DirectX;

			XMVECTOR eyePosition = transform->GetPosition();
			XMVECTOR eyeRotation = transform->GetRotation();

			XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			forwardVector = XMVector3Rotate(forwardVector, eyeRotation);
			XMVECTOR focusPoint = XMVectorAdd(eyePosition, forwardVector);

			return XMMatrixLookAtLH(eyePosition, focusPoint, upVector);
		}
	}
}
