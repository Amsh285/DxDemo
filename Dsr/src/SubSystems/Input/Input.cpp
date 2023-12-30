#include "dsrpch.h"
#include "Input.h"

namespace dsr
{
	namespace subsystems
	{
		Input::Input()
			: m_keyboard(std::make_shared<dsr::inputdevices::Keyboard>()),
			m_mouse(std::make_shared<dsr::inputdevices::Mouse>())
		{
		}
	}
}
