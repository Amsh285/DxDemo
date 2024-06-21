#pragma once

#include "dsrpch.h"
#include "EngineSubSystems/EntityComponentSystem/Component.h"

class CameraControllerComponent : public dsr::ecs::Component
{
public:
	DirectX::XMINT2 MouseRightCenter;
	float MouseRightYaw;
	float MouseRightPitch;

	CameraControllerComponent();
private:
};

