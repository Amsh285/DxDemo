#pragma once

#include "dsrpch.h"
#include "EngineSubSystems/EntityComponentSystem/Component.h"

class CameraControllerComponent : public dsr::ecs::Component
{
public:
	DirectX::XMINT2 MouseMiddleCenter;

	DirectX::XMINT2 MouseRightCenter;
	DirectX::XMFLOAT3 MouseRightRotations;

	CameraControllerComponent();
private:
};

