
#include "dsrpch.h"
#include "WindowApplication.h"

namespace dsr
{
	namespace windows
	{
		std::shared_ptr<WindowApplication> WindowApplication::m_instance;
		std::mutex WindowApplication::m_appMutex;

		std::shared_ptr<WindowApplication> WindowApplication::Get()
		{
			std::lock_guard<std::mutex> guard(m_appMutex);

			if (!m_instance)
				m_instance = std::shared_ptr<WindowApplication>(new WindowApplication());

			return m_instance;
		}

		void WindowApplication::PeekMessages()
		{
			MSG message = { 0 };

			uint32_t counter = 0;

			while (true)
			{
				if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);

					if (message.message == WM_QUIT)
						break;
				}
				
				//std::cout << "updateframe" << counter++ << std::endl;

				dsr::events::UpdateFrameEvent event;
				m_updateFrameEvent.operator()(event);
			}
		}

		void WindowApplication::ShutDown()
		{
			PostQuitMessage(0);
		}
	}
}
