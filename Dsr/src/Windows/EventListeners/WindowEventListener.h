#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"

#include "Windows/WindowApplication.h"

namespace dsr
{
	namespace windows
	{
		class WindowEventListener : public dsr::events::EventListener
		{
		public:
			void OnWindowDestroy(const dsr::events::WindowDestroyEvent& destroyEvent);
		};
	}
}
