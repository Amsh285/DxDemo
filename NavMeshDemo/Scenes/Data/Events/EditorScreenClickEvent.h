#pragma once

#include "dsrpch.h"

#include "Events/Application/InputEvents/MouseEvents.h"

#include "InputDevices/Screen.h"

#include "Scenes/Data/PathSelectType.h"

class EditorScreenClickEvent
{
public:
	dsr::events::MousePosition GetPosition() const { return m_position; }
	dsr::inputdevices::Screen GetScreen() const { return m_screen; }

	PathSelectType GetPathSelectionMode() const { return m_pathSelectionMode; }

	EditorScreenClickEvent(
		const dsr::events::MousePosition& position,
		const dsr::inputdevices::Screen& screen,
		const PathSelectType& selectionMode
	);
private:
	dsr::events::MousePosition m_position;
	dsr::inputdevices::Screen m_screen;

	PathSelectType m_pathSelectionMode;
};

