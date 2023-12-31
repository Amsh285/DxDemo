#include "dsrpch.h"
#include "Input.h"

namespace dsr
{
	namespace input
	{
		Input::Input(
			const std::shared_ptr<dsr::inputdevices::Keyboard>& keyboard,
			const std::shared_ptr<dsr::inputdevices::Mouse>& mouse,
			const std::unordered_map<KeyCode, uint16_t>& keyMap)
			: m_keyboard(keyboard), m_mouse(mouse), m_keyMap(keyMap)
		{
		}
	}
}
