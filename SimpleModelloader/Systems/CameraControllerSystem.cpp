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
	using namespace dsr::ecs;

	std::shared_ptr<CameraControllerComponent> camerControllerData = context.GetComponent<CameraControllerComponent>();
	std::vector<Entity> cameraEntities = context.FindEntitiesByTag("CameraControllerData");

	if (cameraEntities.size() < 1)
		return;

	
	/*if (m_input->GetKeyDown(dsr::input::KeyCode::MouseLeft))
		std::cout << "Chopper down njiaj" << std::endl;

	if (m_input->GetKeyHold(dsr::input::KeyCode::MouseLeft))
		std::cout << "Get to the Chopper!!!" << std::endl;

	if (m_input->GetKeyUp(dsr::input::KeyCode::MouseLeft))
		std::cout << "Chopper Up njiaj" << std::endl;*/


	if (m_input->GetKeyDown(dsr::input::KeyCode::A))
		std::cout << "Chopper down njiaj" << std::endl;

	if (m_input->GetKeyHold(dsr::input::KeyCode::A))
		std::cout << "Get to the Chopper!!!" << std::endl;

	if (m_input->GetKeyUp(dsr::input::KeyCode::A))
		std::cout << "Chopper Up njiaj" << std::endl;
}
