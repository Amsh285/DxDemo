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

		void Keyboard::OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate)
		{
			// works for now. I should check out focus behaviour later.
			// Keyups dont get fired in debug probably because the window loses focus.
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
