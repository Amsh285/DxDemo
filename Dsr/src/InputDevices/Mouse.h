#pragma once

#include "Events/Application/InputEvents/MouseEvents.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/FrameEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace inputdevices
	{
constexpr auto MouseWheelRotationDelta = 120;

		class Mouse : public dsr::events::EventListener
		{
		public:
			float GetMouseWheelRotationDeltaZ() const { return m_mousewheelDeltaZ; }

			dsr::events::MousePosition GetCurrentClientAreaPosition() const { return m_currentClientAreaPosition; }
			dsr::events::MousePosition GetPreviousClientAreaPosition() const { return m_previousClientAreaPosition; }

			bool IsKeyDown(const uint16_t& keyFlags) const;
			bool IsKeyHolding(const uint16_t& keyFlags) const;
			bool IsKeyUp(const uint16_t& keyFlags) const;
			bool IsMoving() const;

			Mouse();

			void OnMouseDown(const dsr::events::MouseDownEvent& mouseDown);
			void OnMouseUp(const dsr::events::MouseUpEvent& mouseUp);
			void OnMouseWheelRotated(const dsr::events::MouseWheelEvent& mouseWheel);
			void OnMouseMove(const dsr::events::MouseMoveEvent& mouseMove);
			void OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus);
			void PrepareNextFrame();
		private:
			float m_mousewheelDeltaZ;
			uint16_t m_currentKeyState, m_previousKeyState;
			dsr::events::MousePosition m_currentClientAreaPosition, m_previousClientAreaPosition;
		};
	}
}
