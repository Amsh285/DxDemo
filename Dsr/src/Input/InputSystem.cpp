#include "dsrpch.h"
#include "InputSystem.h"

namespace dsr
{
	namespace input
	{
		InputSystem::InputSystem()
			: m_keyboard(std::make_shared<dsr::input::Keyboard>()),
			m_mouse(std::make_shared<dsr::input::Mouse>())
		{
		}
	}
}
