#include "dsrpch.h"
#include "Mouse.h"

namespace dsr
{
	namespace inputdevices
	{
		bool Mouse::IsKeyDown(const uint16_t& keyFlags) const
		{

			return (m_currentKeyState & keyFlags) && (~m_previousKeyState & keyFlags);
		}

		bool Mouse::IsKeyHolding(const uint16_t& keyFlags) const
		{
			return (m_currentKeyState & keyFlags) && (m_previousKeyState & keyFlags);
		}

		bool Mouse::IsKeyUp(const uint16_t& keyFlags) const
		{
			return (~m_currentKeyState & keyFlags) && (m_previousKeyState & keyFlags);
		}

		bool Mouse::IsMoving() const
		{
			return !m_currentPosition.Equals(m_previousPosition);
		}

		Mouse::Mouse()
			: m_currentKeyState(0), m_previousKeyState(0)
		{
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

		void Mouse::OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus)
		{
			//this will reset the mouse button states when the application window looses focus
			m_currentKeyState = m_previousKeyState = 0x0000;
		}

		void Mouse::OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate)
		{
			m_previousKeyState = m_currentKeyState;
			m_previousPosition = m_currentPosition;
		}
	}
}
