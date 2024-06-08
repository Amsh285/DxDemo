#include "EditorScreenClickEvent.h"

EditorScreenClickEvent::EditorScreenClickEvent(
	const dsr::events::MousePosition& position,
	const dsr::inputdevices::Screen& screen,
	const PathSelectType& selectionMode
) : m_position(position), m_screen(screen), m_pathSelectionMode(selectionMode)
{
}
