#pragma once

#include "Events/Application/InputEvents/KeyboardEvents.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/FrameEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace inputdevices
	{
		class Keyboard : public dsr::events::EventListener
		{
		public:
			bool IsKeyDown(const uint16_t& keyCode) const;
			bool IsKeyHolding(const uint16_t& keyCode) const;
			bool IsKeyUp(const uint16_t& keyCode) const;

			Keyboard();

			void OnKeyDown(const dsr::events::KeyDownEvent& keyDown);
			void OnKeyUp(const dsr::events::KeyUpEvent& keyUp);
			void OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus);
			void OnUpdateFrameFinished(const dsr::events::UpdateFrameFinishedEvent& updateFinished);
		private:
			uint16_t m_currentState, m_previousState;
		};
	}
}
