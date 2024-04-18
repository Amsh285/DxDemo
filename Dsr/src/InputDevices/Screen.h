#pragma once


#include "Events/Application/WindowEvents.h"
#include "Events/EventListener.h"

namespace dsr
{
	namespace inputdevices
	{
		class Screen : public dsr::events::EventListener
		{
		public:
			int32_t GetClientWidth() const { return m_clientWidth; }
			int32_t GetClientHeight() const { return m_clientHeight; }

			void OnWindowResized(const dsr::events::WindowResizedEvent& resizedEvent);
		private:
			int32_t m_clientWidth;
			int32_t m_clientHeight;
		};
	}
}
