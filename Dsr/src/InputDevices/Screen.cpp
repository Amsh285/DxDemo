#include "dsrpch.h"
#include "Screen.h"

void dsr::inputdevices::Screen::OnWindowResized(const dsr::events::WindowResizedEvent& resizedEvent)
{
	m_clientWidth = resizedEvent.GetClientWidth();
	m_clientHeight = resizedEvent.GetClientHeight();
}
