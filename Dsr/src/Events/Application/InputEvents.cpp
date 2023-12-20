
#include "dsrpch.h"
#include "InputEvents.h"

namespace dsr
{
	namespace events
	{
		KeyDownEvent::KeyDownEvent(const uint8_t& keyCode)
			: m_keyCode(keyCode)
		{
		}

		KeyUpEvent::KeyUpEvent(const uint8_t& keyCode)
			: m_keyCode(keyCode)
		{
		}
	}
}
