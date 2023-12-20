#pragma once

#include "Events/Application/InputEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace input
	{
		class Keyboard : public dsr::events::EventListener
		{
		public:
			bool IsKeyDown(const uint8_t& keyCode);
			bool IsKeyUp(const uint8_t& keyCode);

			Keyboard();

			void HandleKeyDown(const dsr::events::KeyDownEvent& keyDown);
			void HandleKeyUp(const dsr::events::KeyUpEvent& keyUp);
		private:
			std::bitset<256> m_currentState, m_previousState;
		};
	}
}
