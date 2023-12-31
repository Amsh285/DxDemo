
#include "dsrpch.h"
#include "KeyboardEvents.h"

namespace dsr
{
	namespace events
	{
		KeyDownEvent::KeyDownEvent(const uint16_t& keyCode)
			: m_keyCode(keyCode)
		{
		}

		KeyUpEvent::KeyUpEvent(const uint16_t& keyCode)
			: m_keyCode(keyCode)
		{
		}
	}
}
