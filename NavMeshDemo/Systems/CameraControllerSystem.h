#pragma once

#include "dsrpch.h"
#include "Components/CameraControllerComponent.h"

#include "EngineSubSystems/EntityComponentSystem/Entity.h"
#include "EngineSubSystems/EntityComponentSystem/System.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/Input/Input.h"
#include "EngineSubSystems/Time/Time.h"

class CameraControllerSystem : public dsr::ecs::System
{
public:
	virtual std::vector<std::type_index> GetRequiredComponents() const override;

	CameraControllerSystem(
		const std::shared_ptr<dsr::input::Input>& input,
		const std::shared_ptr<dsr::time::Time>& time);

	void Update(const dsr::ecs::EngineContext& context);
private:
	std::shared_ptr<dsr::input::Input> m_input;
	std::shared_ptr<dsr::time::Time> m_time;
};

