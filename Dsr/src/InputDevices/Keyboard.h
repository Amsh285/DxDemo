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

			void PrepareNextFrame();
		private:
			uint16_t m_currentState, m_previousState; // change to std::bitset

			// possible overflow when oring multiple keystates on IsKey...
			// next time use this:
			/*std::bitset<256> keyStates;

			// On KeyDown:
			keyStates.set(static_cast<size_t>(keycode), true);

			// On KeyUp:
			keyStates.set(static_cast<size_t>(keycode), false);
			Und zum Abfragen einfach:

			if (keyStates.test(static_cast<size_t>(keycode))) {
				// Key is pressed
			}
			
			*/
/*namespace dsr
{
	namespace inputdevices
	{
		class Keyboard : public dsr::events::EventListener
		{
		public:
			Keyboard() = default;

			void OnKeyDown(const dsr::events::KeyDownEvent& keyDown)
			{
				keyStates.set(static_cast<size_t>(keyDown.GetKeyCode()), true);
			}

			void OnKeyUp(const dsr::events::KeyUpEvent& keyUp)
			{
				keyStates.set(static_cast<size_t>(keyUp.GetKeyCode()), false);
			}

			void OnLooseFocus(const dsr::events::LooseFocusEvent& looseFocus)
			{
				keyStates.reset();
				prevKeyStates.reset();
			}

			void PrepareNextFrame()
			{
				prevKeyStates = keyStates;
			}

			bool IsKeyDown(uint16_t keyCode) const
			{
				size_t idx = static_cast<size_t>(keyCode);
				return keyStates.test(idx) && !prevKeyStates.test(idx);
			}

			bool IsKeyHolding(uint16_t keyCode) const
			{
				size_t idx = static_cast<size_t>(keyCode);
				return keyStates.test(idx) && prevKeyStates.test(idx);
			}

			bool IsKeyUp(uint16_t keyCode) const
			{
				size_t idx = static_cast<size_t>(keyCode);
				return !keyStates.test(idx) && prevKeyStates.test(idx);
			}

		private:
			std::bitset<256> keyStates;
			std::bitset<256> prevKeyStates;
		};
	}
}*/
		};
	}
}
