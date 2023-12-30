#include "dsrpch.h"
#include "Keyboard.h"

namespace dsr
{
	namespace inputdevices
	{
		Keyboard::Keyboard()
		{
		}

		void Keyboard::OnKeyDown(const dsr::events::KeyDownEvent& keyDown)
		{
			m_currentState[keyDown.GetKeyCode()] = true;
		}

		void Keyboard::OnKeyUp(const dsr::events::KeyUpEvent& keyUp)
		{
			m_currentState[keyUp.GetKeyCode()] = false;
		}

		void Keyboard::OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate)
		{
			m_previousState = m_currentState;
		}

		bool Keyboard::IsKeyDown(const uint8_t& keyCode) const
		{
			return m_currentState[keyCode] && !m_previousState[keyCode];
		}

		bool Keyboard::IsKeyHolding(const uint8_t& keyCode) const
		{
			return m_currentState[keyCode] && m_previousState[keyCode];
		}

		bool Keyboard::IsKeyUp(const uint8_t& keyCode) const
		{
			return !m_currentState[keyCode] && m_previousState[keyCode];
		}
	}
}
