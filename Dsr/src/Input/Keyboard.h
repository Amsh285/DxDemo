#pragma once

#include "Events/Application/InputEvents/KeyboardEvents.h"
#include "Events/Application/WindowEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace input
	{
		class Keyboard : public dsr::events::EventListener
		{
		public:
			bool IsKeyDown(const uint8_t& keyCode);
			bool IsKeyHolding(const uint8_t& keyCode);
			bool IsKeyUp(const uint8_t& keyCode);

			Keyboard();

			void OnKeyDown(const dsr::events::KeyDownEvent& keyDown);
			void OnKeyUp(const dsr::events::KeyUpEvent& keyUp);
			void OnPrepareUpdateFrame(const dsr::events::PrepareUdateFrameEvent& prepareUpdate);
		private:
			std::bitset<256> m_currentState, m_previousState;
		};
	}
}
