#pragma once

#include "Events/IEvent.h"

namespace dsr
{
	namespace events
	{
		class KeyDownEvent : public IEvent
		{
		public:
			uint8_t GetKeyCode() const { return m_keyCode; }

			explicit KeyDownEvent(const uint8_t& keyCode);
		private:
			uint8_t m_keyCode;
		};

		class KeyUpEvent : public IEvent
		{
		public:
			uint8_t GetKeyCode() const { return m_keyCode; }

			explicit KeyUpEvent(const uint8_t& keyCode);
		private:
			uint8_t m_keyCode;
		};
	}
}
