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
			dsr::events::MousePosition GetCurrentPosition() const { return m_currentPosition; }
			dsr::events::MousePosition GetPreviousPosition() const { return m_previousPosition; }

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
			void OnUpdateFrameFinished(const dsr::events::UpdateFrameFinishedEvent& updateFinished);
		private:
			float m_mousewheelDeltaZ;
			uint16_t m_currentKeyState, m_previousKeyState;
			dsr::events::MousePosition m_currentPosition, m_previousPosition;
		};
	}
}
