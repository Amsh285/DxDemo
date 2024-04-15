#pragma once

#include "Events/IEvent.h"

namespace dsr
{
	namespace events
	{
		struct WndProcEvent : public IEvent
		{
			HWND Handle;
			uint32_t Message;
			WPARAM WParam;
			LPARAM LParam;
		};

		class WindowCloseEvent : public IEvent
		{
		};

		class WindowDestroyEvent : public IEvent
		{
		};

		class WindowResizedEvent : public IEvent
		{
		public:
			int32_t GetClientWidth() const { return m_clientWidth; }
			int32_t GetClientHeight() const { return m_clientHeght; }

			WindowResizedEvent(const int32_t& clientWidth, const int32_t& clientHeight)
				: m_clientWidth(clientWidth), m_clientHeght(clientHeight)
			{
			}
		private:
			int32_t m_clientWidth;
			int32_t m_clientHeght;
		};

		class AquiredFocusEvent : public IEvent
		{
		};

		class LooseFocusEvent : public IEvent
		{
		};
	}
}
