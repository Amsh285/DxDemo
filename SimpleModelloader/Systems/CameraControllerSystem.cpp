#include "CameraControllerSystem.h"



std::vector<std::type_index> CameraControllerSystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(CameraControllerComponent)), std::type_index(typeid(dsr::ecs::TransformComponent))};
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
	std::shared_ptr<TransformComponent> cameraTransform = context.GetComponent<TransformComponent>();

	int16_t deltaZ = m_input->GetMouseWheelRotationDeltaZ();

	if (m_input->GetKeyDown(KeyCode::MouseMiddle))
	{
		MousePosition position = m_input->GetMouse()->GetCurrentPosition();
		cameraControllerData->MouseMiddleCenter = XMINT2(position.X, position.Y);
	}
	else if (m_input->GetKeyHold(KeyCode::MouseMiddle))
	{
		constexpr float speed = 0.001f;
		constexpr XMINT2 threshold = XMINT2(5, 5);

		MousePosition position = m_input->GetMouse()->GetCurrentPosition();

		XMINT2 center = cameraControllerData->MouseMiddleCenter;
		XMINT2 currentPosition = XMINT2(position.X, position.Y);
		XMINT2 delta = XMINT2(currentPosition.x - center.x, currentPosition.y - center.y);

		bool movesOnXAxis = abs(delta.x) >= threshold.x, movesOnYAxis = abs(delta.y) >= threshold.y;
		bool movesOnAnyAxis = movesOnXAxis || movesOnYAxis;

		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(cameraControllerData->MouseRightPitch),
			XMConvertToRadians(cameraControllerData->MouseRightYaw),
			0.0f);
		forward = XMVector3Rotate(forward, quaternion);
		up = XMVector3Rotate(up, quaternion);
		XMVECTOR side = XMVector3Cross(up, forward);

		if (movesOnXAxis)
		{
			side = XMVectorScale(side, delta.x * speed);
			side = XMVectorNegate(side);
			cameraTransform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), side));
		}

		if (movesOnYAxis)
		{
			up = XMVectorScale(up, delta.y * speed);
			cameraTransform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), up));
		}

		if (movesOnAnyAxis)
		{
			cameraControllerData->MouseMiddleCenter = XMINT2(position.X, position.Y);
		}
	}
	else if (m_input->GetKeyDown(KeyCode::MouseRight))
	{
		MousePosition position = m_input->GetMouse()->GetCurrentPosition();
		cameraControllerData->MouseRightCenter = XMINT2(position.X, position.Y);
	}
	else if (m_input->GetKeyHold(KeyCode::MouseRight))
	{
		constexpr float speed = 0.05f;
		constexpr XMINT2 threshold = XMINT2(5, 5);

		MousePosition position = m_input->GetMouse()->GetCurrentPosition();

		XMINT2 center = cameraControllerData->MouseRightCenter;
		XMINT2 currentPosition = XMINT2(position.X, position.Y);
		XMINT2 delta = XMINT2(currentPosition.x - center.x, currentPosition.y - center.y);

		bool movesOnXAxis = abs(delta.x) >= threshold.x, movesOnYAxis = abs(delta.y) >= threshold.y;
		bool movesOnAnyAxis = movesOnXAxis || movesOnYAxis;

		if (movesOnXAxis)
			cameraControllerData->MouseRightYaw = fmod(cameraControllerData->MouseRightYaw + static_cast<float>(delta.x) * speed, 360.0f);

		if (movesOnYAxis)
			cameraControllerData->MouseRightPitch = fmod(cameraControllerData->MouseRightPitch + static_cast<float>(delta.y) * speed, 360.0f);

		if (movesOnAnyAxis)
		{
			cameraTransform->SetRotation(
				XMQuaternionRotationRollPitchYaw(
					XMConvertToRadians(cameraControllerData->MouseRightPitch),
					XMConvertToRadians(cameraControllerData->MouseRightYaw),
					0.0f));

			cameraControllerData->MouseRightCenter = XMINT2(position.X, position.Y);
		}
	}
	else if (abs(deltaZ) >= 0.1f)
	{
		constexpr float speed = 0.1f;

		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(cameraControllerData->MouseRightPitch),
			XMConvertToRadians(cameraControllerData->MouseRightYaw),
			0.0f);

		forward = XMVector3Rotate(forward, rotation);
		forward = XMVectorScale(forward, deltaZ * speed);

		XMVECTOR position = XMVectorAdd(forward, cameraTransform->GetPosition());
		cameraTransform->SetPosition(position);
	}
}
