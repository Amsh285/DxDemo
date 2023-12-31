#pragma once

#include "InputDevices/Keyboard.h"
#include "InputDevices/Mouse.h"

#include "SubSystems/Input/KeyCode.h"

namespace dsr
{
	namespace input
	{
		class Input
		{
		public:
			Input();
		protected:
			std::shared_ptr<dsr::inputdevices::Keyboard> m_keyboard;
			std::shared_ptr<dsr::inputdevices::Mouse> m_mouse;
			std::unordered_map<KeyCode, uint16_t> m_keyMap;
		};
	}
}
