#include "dsrpch.h"
#include "EventDispatcher.h"

namespace dsr
{
	EventDispatcher::EventDispatcher(
		const std::shared_ptr<dsr::windows::Window>& window,
		const std::shared_ptr<dsr::windows::WindowApplication>& windowApplication)
		: m_window(window), m_windowApplication(windowApplication)
	{
	}
}
