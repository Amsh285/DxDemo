#include "dsrpch.h"
#include "Mouse.h"

namespace dsr
{
	namespace input
	{
		bool Mouse::IsKeyDown(const uint16_t& keyCode) const
		{
			return m_currentKeyState[keyCode] && !m_previousKeyState[keyCode];
		}

		bool Mouse::IsKeyHolding(const uint16_t& keyCode) const
		{
			return m_currentKeyState[keyCode] && m_previousKeyState[keyCode];
		}

		bool Mouse::IsKeyUp(const uint16_t& keyCode) const
		{
			return !m_currentKeyState[keyCode] && m_previousKeyState[keyCode];
		}

		bool Mouse::IsMoving() const
		{
			return !m_currentPosition.Equals(m_previousPosition);
		}

		void Mouse::OnMouseDown(const dsr::events::MouseDownEvent& mouseDown)
		{
			m_currentKeyState = mouseDown.GetKeyCode();
			m_currentPosition = mouseDown.GetPosition();
		}

		void Mouse::OnMouseUp(const dsr::events::MouseUpEvent& mouseUp)
		{
			m_currentKeyState = mouseUp.GetKeyCode();
			m_currentPosition = mouseUp.GetPosition();
		}

		void Mouse::OnMouseMove(const dsr::events::MouseMoveEvent& mouseMove)
		{
			m_currentPosition = mouseMove.GetPosition();
		}

		void Mouse::OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate)
		{
			m_previousKeyState = m_currentKeyState;
			m_previousPosition = m_currentPosition;
		}
	}
}
