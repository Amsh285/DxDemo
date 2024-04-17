#include "dsrpch.h"
#include "Keyboard.h"

namespace dsr
{
	namespace inputdevices
	{
		Keyboard::Keyboard()
			: m_currentState(0), m_previousState(0)
		{
		}

		void Keyboard::OnKeyDown(const dsr::events::KeyDownEvent& keyDown)
		{
			m_currentState |= keyDown.GetKeyCode();
		}

		void Keyboard::OnKeyUp(const dsr::events::KeyUpEvent& keyUp)
		{
			m_currentState &= ~keyUp.GetKeyCode();
		}

		void Keyboard::OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus)
		{
			// this will interupt keydowns and prevent keyups when losing keyboardfocus
			// another solution could be to manually trigger key ups for all keys that are currently down
			// but this could lead to unexpected behaviour.
			// Therefore when loosing the keyboard-focus, the keyboard will be reset to a clean state.
			// It is up to Developers to handle key up events with
			// the possibility of the window loosing focus appropriately.
			m_previousState = m_currentState = 0x0000;
		}

		void Keyboard::PrepareNextFrame()
		{
			m_previousState = m_currentState;
		}

		bool Keyboard::IsKeyDown(const uint16_t& keyCode) const
		{
			return (m_currentState & keyCode) && (~m_previousState & keyCode);
		}

		bool Keyboard::IsKeyHolding(const uint16_t& keyCode) const
		{
			return (m_currentState & keyCode) && (m_previousState & keyCode);
		}

		bool Keyboard::IsKeyUp(const uint16_t& keyCode) const
		{
			return (~m_currentState & keyCode) && (m_previousState & keyCode);
		}
	}
}
