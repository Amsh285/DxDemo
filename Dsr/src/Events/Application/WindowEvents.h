#pragma once

#include "Events/IEvent.h"

namespace dsr
{
	namespace events
	{
		class WindowCloseEvent : public IEvent
		{
		};

		class WindowDestroyEvent : public IEvent
		{
		};
	}
}
