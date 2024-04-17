#include "dsrpch.h"
#include "EventDispatcher.h"

namespace dsr
{
	EventDispatcher::EventDispatcher(const std::shared_ptr<dsr::windows::Window>& window)
		: m_window(window)
	{
	}
}
