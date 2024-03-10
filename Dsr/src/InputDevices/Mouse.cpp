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
			return !m_currentClientAreaPosition.Equals(m_previousClientAreaPosition);
		}

		Mouse::Mouse()
			: m_currentKeyState(0), m_previousKeyState(0), m_mousewheelDeltaZ(0)
		{
		}

		void Mouse::OnMouseDown(const dsr::events::MouseDownEvent& mouseDown)
		{
			m_currentKeyState = mouseDown.GetKeyCode();
			m_currentClientAreaPosition = mouseDown.GetPosition();
		}

		void Mouse::OnMouseUp(const dsr::events::MouseUpEvent& mouseUp)
		{
			m_currentKeyState = mouseUp.GetKeyCode();
			m_currentClientAreaPosition = mouseUp.GetPosition();
		}

		void Mouse::OnMouseWheelRotated(const dsr::events::MouseWheelEvent& mouseWheel)
		{
			m_mousewheelDeltaZ = mouseWheel.GetDeltaZ() / MouseWheelRotationDelta;
		}

		void Mouse::OnMouseMove(const dsr::events::MouseMoveEvent& mouseMove)
		{
			m_currentClientAreaPosition = mouseMove.GetPosition();
		}

		void Mouse::OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus)
		{
			//this will reset the mouse button states when the application window looses focus
			m_currentKeyState = m_previousKeyState = 0x0000;
		}

		void Mouse::OnUpdateFrameFinished(const dsr::events::UpdateFrameFinishedEvent& updateFinished)
		{
			m_previousKeyState = m_currentKeyState;
			m_previousClientAreaPosition = m_currentClientAreaPosition;
			m_mousewheelDeltaZ = 0.0f;
		}
	}
}
