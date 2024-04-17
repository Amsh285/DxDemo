#pragma once

#include "EngineSubSystems/Time/FrameDelta.h"
#include "EngineSubSystems/Time/Time.h"

#include "Events/EventEmitter.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/FrameEvents.h"


namespace dsr
{
	namespace windows
	{
		class WindowApplication
		{
		public:
			static std::shared_ptr<WindowApplication> Get();

			EventRegisterType<const dsr::events::UpdateFrameEvent&>& GetUpdateFrameEventRegister() { return m_updateFrameEmitter; }
			EventRegisterType<const dsr::events::UpdateFrameFinishedEvent&>& GetUpdateFrameFinishedRegister() { return m_updateFrameFinishedEmitter; }

			std::shared_ptr<dsr::time::Time> GetTime() const { return m_time; }

			void PeekMessages();
			void ShutDown();
		private:
			WindowApplication();

			EventEmitterType<const dsr::events::UpdateFrameEvent&> m_updateFrameEmitter;
			EventEmitterType<const dsr::events::UpdateFrameFinishedEvent&> m_updateFrameFinishedEmitter;

			static std::shared_ptr<WindowApplication> m_instance;
			static std::mutex m_appMutex;

			std::shared_ptr<dsr::time::Time> m_time;
		};
	}
}
