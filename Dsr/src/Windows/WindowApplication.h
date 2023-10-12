#pragma once

#include "Events/EventEmitter.h"
#include "Events/Application/WindowEvents.h"

namespace dsr
{
	namespace windows
	{
		class WindowApplication
		{
		public:
			static std::shared_ptr<WindowApplication> Get();

			EventRegisterType<const dsr::events::UpdateFrameEvent&>& GetUpdateFrameEventRegister() { return m_updateFrameEvent; }

			void PeekMessages();
			void ShutDown();
		private:
			WindowApplication()
			{
			}

			EventEmitterType<const dsr::events::UpdateFrameEvent&> m_updateFrameEvent;

			static std::shared_ptr<WindowApplication> m_instance;
			static std::mutex m_appMutex;
		};
	}
}
