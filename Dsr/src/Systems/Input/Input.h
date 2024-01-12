#pragma once

#include "InputDevices/Keyboard.h"
#include "InputDevices/Mouse.h"

#include "Systems/Input/KeyCode.h"

namespace dsr
{
	namespace input
	{
		class Input
		{
		public:
			std::shared_ptr<dsr::inputdevices::Keyboard> GetKeyboard() const { return m_keyboard; }
			std::shared_ptr<dsr::inputdevices::Mouse> GetMouse() const { return m_mouse; }

			Input(
				const std::shared_ptr<dsr::inputdevices::Keyboard>& keyboard,
				const std::shared_ptr<dsr::inputdevices::Mouse>& mouse,
				const std::unordered_map<KeyCode, uint16_t>& keyMap
			);
			Input(const Input& other) = delete;
			Input& operator=(const Input& other) = delete;
		protected:
			std::shared_ptr<dsr::inputdevices::Keyboard> m_keyboard;
			std::shared_ptr<dsr::inputdevices::Mouse> m_mouse;
			std::unordered_map<KeyCode, uint16_t> m_keyMap;
		};
	}
}
