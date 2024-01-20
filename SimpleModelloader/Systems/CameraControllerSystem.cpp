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
		cameraControllerData->MouseMiddleCenter = XMINT2(position.X, position.Y);
	}
	else if (m_input->GetKeyHold(KeyCode::MouseMiddle))
	{
		constexpr float speed = 0.01f;
		constexpr XMINT2 threshold = XMINT2(5, 5);

		MousePosition position = m_input->GetMouse()->GetCurrentPosition();

		XMINT2 center = cameraControllerData->MouseMiddleCenter;
		XMINT2 currentPosition = XMINT2(position.X, position.Y);
		XMINT2 delta = XMINT2(currentPosition.x - center.x, currentPosition.y - center.y);

		XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		
		bool movesOnXAxis = abs(delta.x) >= threshold.x, movesOnYAxis = abs(delta.y) >= threshold.y;
		bool movesOnAnyAxis = movesOnXAxis || movesOnYAxis;

		if (movesOnXAxis)
			translation = XMVectorSetX(translation, delta.x * (-1));

		if(movesOnYAxis)
			translation = XMVectorSetY(translation, delta.y);

		translation = XMVector2Normalize(translation);

		std::shared_ptr<TransformComponent> cameraTransform = context.GetComponentFrom<TransformComponent>(cameraEntities[0]);
		cameraTransform->SetPosition(XMVectorAdd(cameraTransform->GetPosition(), translation * speed));

		if (movesOnAnyAxis)
			cameraControllerData->MouseMiddleCenter = XMINT2(position.X, position.Y);
	}


}
