#include "CameraControllerSystem.h"



std::vector<std::type_index> CameraControllerSystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(CameraControllerComponent)) };
}

CameraControllerSystem::CameraControllerSystem(const std::shared_ptr<dsr::input::Input>& input)
	: dsr::ecs::System(std::type_index(typeid(CameraControllerSystem))),
	m_input(input)
{
	OnUpdate = std::bind(&CameraControllerSystem::Update, this, std::placeholders::_1);
}

void CameraControllerSystem::Update(const dsr::ecs::EngineContext& context)
{
	using namespace DirectX;

	using namespace dsr::ecs;
	using namespace dsr::events;
	using namespace dsr::input;

	std::shared_ptr<CameraControllerComponent> cameraControllerData = context.GetComponent<CameraControllerComponent>();
	std::vector<Entity> cameraEntities = context.FindEntitiesByTag("Camera");

	if (cameraEntities.size() < 1)
		return;

	XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	if (m_input->GetKeyDown(KeyCode::MouseMiddle))
	{
		MousePosition position = m_input->GetMouse()->GetCurrentPosition();
		cameraControllerData->MouseMiddleCenter = XMVectorSet(position.X, position.Y, 0.0f, 0.0f);
	}
	else if (m_input->GetKeyHold(KeyCode::MouseMiddle))
	{
		constexpr float speed = 0.0001f;

		MousePosition position = m_input->GetMouse()->GetCurrentPosition();
		XMVECTOR targetPosition = XMVectorSet(position.X, position.Y, 0.0f, 0.0f);

		XMVECTOR targetDirection = XMVectorSubtract(targetPosition, cameraControllerData->MouseMiddleCenter);
		XMVECTOR targetDirectionNorm = DirectX::XMVector3Normalize(targetDirection);

		//Todo: use pixel deltas instead and move only after a certain threshold

		/*if (XMVectorGetX(XMVector3Length(targetDirectionNorm)) > 0.2f)
		{
			std::shared_ptr<TransformComponent> cameraTransform = context.GetComponentFrom<TransformComponent>(cameraEntities[0]);
			XMVECTOR cameraPosition = cameraTransform->GetPosition();

			XMVECTOR newCameraPosition = XMVectorAdd(cameraPosition, targetDirectionNorm * speed);
			cameraTransform->SetPosition(newCameraPosition);
			cameraControllerData->MouseMiddleCenter = newCameraPosition;
		}*/
	}

	
}
