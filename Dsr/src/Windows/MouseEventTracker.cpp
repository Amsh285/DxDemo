#include "dsrpch.h"
#include "MouseEventTracker.h"

namespace dsr
{
	namespace windows
	{
		MouseEventTracker::MouseEventTracker(const HWND windowHandle)
			: m_tracking(false), m_windowHandle(windowHandle)
		{
		}

		void MouseEventTracker::OnMouseMove(const dsr::events::MouseMoveEvent& moveEvent)
		{
			if (!m_tracking)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.hwndTrack = m_windowHandle;
				tme.dwHoverTime = HOVER_DEFAULT;
				
				m_tracking = TrackMouseEvent(&tme);
			}
		}

		void MouseEventTracker::OnMouseHover(const dsr::events::MouseHoverEvent& hoverEvent)
		{
			m_tracking = false;
		}

		void MouseEventTracker::OnMouseLeave(const dsr::events::MouseLeaveEvent& leaveEvent)
		{
			m_tracking = false;
		}
	}
}
