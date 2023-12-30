#pragma once

#include "Events/Application/InputEvents/MouseEvents.h"
#include "Events/Application/WindowEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace inputdevices
	{
		class Mouse : public dsr::events::EventListener
		{
		public:
			const dsr::events::MousePosition& GetCurrentPosition() const { return m_currentPosition; }
			const dsr::events::MousePosition& GetPreviousPosition() const { return m_previousPosition; }

			bool IsKeyDown(const uint16_t& keyFlags) const;
			bool IsKeyHolding(const uint16_t& keyFlags) const;
			bool IsKeyUp(const uint16_t& keyFlags) const;
			bool IsMoving() const;

			void OnMouseDown(const dsr::events::MouseDownEvent& mouseDown);
			void OnMouseUp(const dsr::events::MouseUpEvent& mouseUp);
			void OnMouseMove(const dsr::events::MouseMoveEvent& mouseMove);
			void OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate);
		private:
			uint16_t m_currentKeyState, m_previousKeyState;
			dsr::events::MousePosition m_currentPosition, m_previousPosition;
		};
	}
}