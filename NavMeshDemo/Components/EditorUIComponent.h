#pragma once

#include "dsrpch.h"
#include "EngineSubSystems/EntityComponentSystem/Component.h"

class EditorUIComponent : public dsr::ecs::Component
{
public:
	int RaycastMode;
	bool ShowColliders;

	EditorUIComponent();
private:
};

