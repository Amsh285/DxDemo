#include "dsrpch.h"
#include "WindowEventListener.h"

namespace dsr
{
	namespace windows
	{
		void WindowEventListener::OnWindowDestroy(const dsr::events::WindowDestroyEvent& destroyEvent)
		{
			std::shared_ptr<dsr::windows::WindowApplication> application = dsr::windows::WindowApplication::Get();
			application->ShutDown();
		}
	}
}
