#pragma once

#include "Events/IEvent.h"

#include "Infrastructure/Duration.h"

namespace dsr
{
	namespace events
	{
		class FrameEvent : public IEvent
		{
		public:
			Duration GetDeltaTime() const { return m_deltaTime; }

			FrameEvent(const Duration& deltaTime)
				: m_deltaTime(deltaTime)
			{
			}
		private:
			Duration m_deltaTime;
		};

		class UpdateFrameEvent : public FrameEvent
		{
		public:
			UpdateFrameEvent(const Duration& deltaTime)
				: FrameEvent(deltaTime)
			{
			}
		};

		class UpdateFrameFinishedEvent : public FrameEvent
		{
		public:
			UpdateFrameFinishedEvent(const Duration& deltaTime)
				: FrameEvent(deltaTime)
			{
			}
		};
	}
}
