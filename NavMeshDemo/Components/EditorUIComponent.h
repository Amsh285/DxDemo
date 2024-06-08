#pragma once

#include "dsrpch.h"
#include "EngineSubSystems/EntityComponentSystem/Component.h"

class EditorUIComponent : public dsr::ecs::Component
{
public:
	int PathSelectMode;
	bool ShowColliders;

	EditorUIComponent();
private:
};
