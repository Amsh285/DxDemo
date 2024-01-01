#pragma once

#include "Events/EventListener.h"
#include "Events/Application/InputEvents/MouseEvents.h"

namespace dsr
{
	namespace windows
	{
		class MouseEventTracker : public dsr::events::EventListener
		{
		public:
			MouseEventTracker(const HWND windowHandle);
			MouseEventTracker(const MouseEventTracker& other) = delete;
			MouseEventTracker& operator=(const MouseEventTracker& other) = delete;

			void OnMouseMove(const dsr::events::MouseMoveEvent& moveEvent);
			void OnMouseHover(const dsr::events::MouseHoverEvent& hoverEvent);
			void OnMouseLeave(const dsr::events::MouseLeaveEvent& leaveEvent);
		private:
			bool m_tracking;
			HWND m_windowHandle;
		};
	}
}
