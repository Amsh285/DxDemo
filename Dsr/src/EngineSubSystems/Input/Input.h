#pragma once

#include "InputDevices/Keyboard.h"
#include "InputDevices/Mouse.h"
#include "InputDevices/Screen.h"

#include "EngineSubSystems/Input/KeyCode.h"

namespace dsr
{
	namespace input
	{
		enum class KeyState
		{
			Down,
			Hold,
			Up
		};

		class Input
		{
		public:
			std::shared_ptr<dsr::inputdevices::Keyboard> GetKeyboard() const { return m_keyboard; }
			std::shared_ptr<dsr::inputdevices::Mouse> GetMouse() const { return m_mouse; }
			std::shared_ptr<dsr::inputdevices::Screen> GetScreen() const { return m_screen; }

			float GetMouseWheelRotationDeltaZ() const { return m_mouse->GetMouseWheelRotationDeltaZ(); }

			bool GetKeyDown(const KeyCode& key) const;
			bool GetKeyHold(const KeyCode& key) const;
			bool GetKeyUp(const KeyCode& key) const;

			Input(
				const std::shared_ptr<dsr::inputdevices::Keyboard>& keyboard,
				const std::shared_ptr<dsr::inputdevices::Mouse>& mouse,
				const std::shared_ptr<dsr::inputdevices::Screen>& screen,
				const std::unordered_map<KeyCode, uint16_t>& keyMap
			);
			Input(const Input& other) = delete;
			Input& operator=(const Input& other) = delete;
		private:
			bool GetKey(const KeyCode& key, const KeyState& state) const;

			std::shared_ptr<dsr::inputdevices::Keyboard> m_keyboard;
			std::shared_ptr<dsr::inputdevices::Mouse> m_mouse;
			std::shared_ptr<dsr::inputdevices::Screen> m_screen;
			std::unordered_map<KeyCode, uint16_t> m_keyMap;
		};
	}
}
