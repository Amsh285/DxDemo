#pragma once

#include "Events/EventListener.h"
#include "Events/Application/FrameEvents.h"

#include "Infrastructure/Duration.h"

namespace dsr
{
	namespace time
	{
		class Time : public dsr::events::EventListener
		{
		public:
			dsr::Duration GetDeltaTime() const { return m_deltaTime; }
			void SetDeltaTime(const dsr::Duration& deltaTime) { m_deltaTime = deltaTime; }

		private:
			dsr::Duration m_deltaTime;
		};
	}
}
