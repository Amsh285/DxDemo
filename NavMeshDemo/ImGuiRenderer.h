#pragma once

#include "dsrpch.h"
#include "Events/EventListener.h"
#include "Events/Application/FrameEvents.h"

class ImGuiRenderer : public dsr::events::EventListener
{
public:
	void OnPrepareUpdate(const dsr::events::PrepareUpdateFrameEvent& prepareUpdateEvent);
	void OnUpdate(const dsr::events::UpdateFrameEvent& updateFrameEvent);
private:
};

