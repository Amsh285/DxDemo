#include "dsrpch.h"
#include "DsrApplication.h"

namespace dsr
{
	void DsrApplication::WindowManager::OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent)
	{
		using namespace dsr::windows;

		std::shared_ptr<WindowApplication> application = WindowApplication::Get();
		application->ShutDown();
	}

	void DsrApplication::WindowManager::OnWindowResize(const events::WindowResizedEvent& resizeEvent)
	{
	}

	DsrApplication::WindowManager::WindowManager(
		const std::shared_ptr<windows::Window>& window,
		const std::shared_ptr<directX::Direct3dDevice>& device)
		: m_window(window), m_device(device)
	{
	}

	void DsrApplication::Initialize()
	{
		m_device = directX::Direct3dDevice::Create(m_window);
		m_renderer = std::make_shared<dsr::directX::rendering::Direct3dRenderer>(m_device);

		m_windowManager = std::make_shared<WindowManager>(m_window, m_device);

		// Additional initialization logic if needed
	}

	DsrApplication::DsrApplication(
		const std::wstring& title,
		const int& x, const int& y,
		const int& width, const int& height)
		: m_window(std::make_shared<windows::Window>(windows::WindowData(title, x, y, width, height))),
		m_windowApplication(windows::WindowApplication::Get())
	{
	}
}
