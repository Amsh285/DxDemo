#include "CameraControllerSystem.h"

std::vector<std::type_index> CameraControllerSystem::GetRequiredComponents() const
{
	return { std::type_index(typeid(CameraControllerComponent)), std::type_index(typeid(dsr::ecs::TransformComponent)) };
}

CameraControllerSystem::CameraControllerSystem(
	const std::shared_ptr<dsr::input::Input>& input,
	const std::shared_ptr<dsr::time::Time>& time)
	: dsr::ecs::System(std::type_index(typeid(CameraControllerSystem))),
	m_forward(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
	m_input(input),
	m_time(time),
	m_mouseMiddleDeltaTime(0.0f),
	m_mouseMiddleDelta(DirectX::XMINT2(0, 0)),
	m_mouseMiddleReferencePoint(DirectX::XMINT2(0, 0))
{
	OnUpdate = std::bind(&CameraControllerSystem::Update, this, std::placeholders::_1);
}

void CameraControllerSystem::Update(const dsr::ecs::EngineContext& context)
{
	using namespace DirectX;

	using namespace dsr::ecs;
	using namespace dsr::events;
	using namespace dsr::input;

	constexpr float frameRateLimit = 1.0f / 120.0f;

	std::shared_ptr<CameraControllerComponent> cameraControllerData = context.GetComponent<CameraControllerComponent>();
	std::shared_ptr<TransformComponent> cameraTransform = context.GetComponent<TransformComponent>();

	int16_t deltaZ = m_input->GetMouseWheelRotationDeltaZ();

	if (m_input->GetKeyDown(KeyCode::MouseMiddle))
	{
		const MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();
		m_mouseMiddleReferencePoint.x = position.X;
		m_mouseMiddleReferencePoint.y = position.Y;
		m_mouseMiddleDeltaTime = 0.0f; // Reset the accumulated time
		m_mouseMiddleDelta = { 0, 0 }; // Reset accumulated deltas
	}
	else if (m_input->GetKeyHold(KeyCode::MouseMiddle))
	{
		// consider using a speedfactor
		constexpr float speed = 100.0f;
		constexpr XMINT2 sensibilityThreshold = XMINT2(8, 8);
		constexpr XMINT2 sensibilityRejectionThreshold = XMINT2(2, 2);

		MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();
		m_mouseMiddleDelta.x += position.X - m_mouseMiddleReferencePoint.x;
		m_mouseMiddleDelta.y += position.Y - m_mouseMiddleReferencePoint.y;
		m_mouseMiddleReferencePoint.x = position.X;
		m_mouseMiddleReferencePoint.y = position.Y;

		float deltaTimeSeconds = m_time->GetDeltaTime()
			.Capped(std::chrono::duration<float>(frameRateLimit))
			.Seconds();

		m_mouseMiddleDeltaTime += deltaTimeSeconds;

		if (m_mouseMiddleDeltaTime < frameRateLimit)
			return;

		XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(cameraControllerData->MouseRightPitch),
			XMConvertToRadians(cameraControllerData->MouseRightYaw),
			0.0f);
		XMVECTOR forward = XMVector3Rotate(m_forward, quaternion);
		XMVECTOR up = XMVector3Rotate(m_up, quaternion);
		XMVECTOR side = XMVector3Cross(up, forward);

		bool movesOnXAxis = abs(m_mouseMiddleDelta.x) >= sensibilityThreshold.x;
		bool movesOnYAxis = abs(m_mouseMiddleDelta.y) >= sensibilityThreshold.y;

		if(movesOnXAxis)
		{
			side = XMVectorScale(side, m_mouseMiddleDelta.x / std::abs(m_mouseMiddleDelta.x) * speed * m_mouseMiddleDeltaTime);
			side = XMVectorNegate(side);
			cameraTransform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), side));
			m_mouseMiddleDelta.x = 0;
		}

		if(movesOnYAxis)
		{
			up = XMVectorScale(up, m_mouseMiddleDelta.y / std::abs(m_mouseMiddleDelta.y) * speed * m_mouseMiddleDeltaTime);
			cameraTransform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), up));
			m_mouseMiddleDelta.y = 0;
		}

		if(abs(m_mouseMiddleDelta.x) < sensibilityRejectionThreshold.x)
			m_mouseMiddleDelta.x = 0;
		if(abs(m_mouseMiddleDelta.y) < sensibilityRejectionThreshold.y)
			m_mouseMiddleDelta.y = 0;
		
		m_mouseMiddleDeltaTime = 0.0f; // Reset the accumulated time
	}
	else if(m_input->GetKeyUp(KeyCode::MouseMiddle))
	{
		
	}
	else if (m_input->GetKeyDown(KeyCode::MouseRight))
	{
		MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();
		cameraControllerData->MouseRightCenter = XMINT2(position.X, position.Y);
	}
	else if (m_input->GetKeyHold(KeyCode::MouseRight))
	{
		constexpr float speed = 200.0f;
		constexpr XMINT2 threshold = XMINT2(5, 5);

		MousePosition position = m_input->GetMouse()->GetCurrentClientAreaPosition();

		XMINT2 center = cameraControllerData->MouseRightCenter;
		XMINT2 currentPosition = XMINT2(position.X, position.Y);
		XMINT2 delta = XMINT2(currentPosition.x - center.x, currentPosition.y - center.y);

		bool movesOnXAxis = abs(delta.x) >= threshold.x, movesOnYAxis = abs(delta.y) >= threshold.y;
		bool movesOnAnyAxis = movesOnXAxis || movesOnYAxis;

		float deltaTimeSeconds = m_time->GetDeltaTime()
			.Capped(std::chrono::duration<float>(1))
			.Seconds();

		// Todo: check delta values low deltas lead to stuttering
		// maybe try to increase threshold
		if (movesOnXAxis)
			cameraControllerData->MouseRightYaw = fmod(cameraControllerData->MouseRightYaw + static_cast<float>(delta.x) * speed * deltaTimeSeconds, 360.0f);

		if (movesOnYAxis)
			cameraControllerData->MouseRightPitch = fmod(cameraControllerData->MouseRightPitch + static_cast<float>(delta.y) * speed * deltaTimeSeconds, 360.0f);

		if (movesOnAnyAxis)
		{
			// probably winapi related error mousewheel rotation sets mouse position for some reason...

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
		constexpr float speed = 3000.0f;
		float deltaTimeSeconds = m_time->GetDeltaTime()
			.Capped(std::chrono::duration<float>(1))
			.Seconds();

		XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(cameraControllerData->MouseRightPitch),
			XMConvertToRadians(cameraControllerData->MouseRightYaw),
			0.0f);

		forward = XMVector3Rotate(forward, rotation);
		forward = XMVectorScale(forward, deltaZ * speed * deltaTimeSeconds);

		XMVECTOR position = XMVectorAdd(forward, cameraTransform->GetPosition());
		cameraTransform->SetPosition(position);
	}
}
