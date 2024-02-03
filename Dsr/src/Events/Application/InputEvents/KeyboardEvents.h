#pragma once

#include "Events/IEvent.h"

namespace dsr
{
	namespace events
	{
		class KeyDownEvent : public IEvent
		{
		public:
			const uint16_t& GetKeyCode() const { return m_keyCode; }

			explicit KeyDownEvent(const uint16_t& keyCode);
		private:
			uint16_t m_keyCode;
		};

		class KeyUpEvent : public IEvent
		{
		public:
			const uint16_t& GetKeyCode() const { return m_keyCode; }

			explicit KeyUpEvent(const uint16_t& keyCode);
		private:
			uint16_t m_keyCode;
		};
	}
}
