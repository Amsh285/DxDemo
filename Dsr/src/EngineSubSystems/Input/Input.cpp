#include "dsrpch.h"
#include "Input.h"
#include "Input.h"

namespace dsr
{
	namespace input
	{
		bool Input::GetKeyDown(const KeyCode& key) const
		{
			return GetKey(key, KeyState::Down);
		}

		bool Input::GetKeyHold(const KeyCode& key) const
		{
			return GetKey(key, KeyState::Hold);
		}

		bool Input::GetKeyUp(const KeyCode& key) const
		{
			return GetKey(key, KeyState::Up);
		}

		Input::Input(
			const std::shared_ptr<dsr::inputdevices::Keyboard>& keyboard,
			const std::shared_ptr<dsr::inputdevices::Mouse>& mouse,
			const std::shared_ptr<dsr::inputdevices::Screen>& screen,
			const std::unordered_map<KeyCode, uint16_t>& keyMap)
			: m_keyboard(keyboard), m_mouse(mouse), m_screen(screen), m_keyMap(keyMap)
		{
		}

		bool Input::GetKey(const KeyCode& key, const KeyState& state) const
		{
			// maybe log here
			if (m_keyMap.find(key) == m_keyMap.end())
				return false;

			switch (state)
			{
			case KeyState::Down:
			{
				switch (key)
				{
				case KeyCode::MouseLeft:
				case KeyCode::MouseMiddle:
				case KeyCode::MouseRight:
					return m_mouse->IsKeyDown(m_keyMap.at(key));
				default:
					return m_keyboard->IsKeyDown(m_keyMap.at(key));
				}
			}
			case KeyState::Hold:
				switch (key)
				{
				case KeyCode::MouseLeft:
				case KeyCode::MouseMiddle:
				case KeyCode::MouseRight:
					return m_mouse->IsKeyHolding(m_keyMap.at(key));
				default:
					return m_keyboard->IsKeyHolding(m_keyMap.at(key));
				}
			case KeyState::Up:
				switch (key)
				{
				case KeyCode::MouseLeft:
				case KeyCode::MouseMiddle:
				case KeyCode::MouseRight:
					return m_mouse->IsKeyUp(m_keyMap.at(key));
				default:
					return m_keyboard->IsKeyUp(m_keyMap.at(key));
				}
			default:
				return false;
			}
		}
	}
}
