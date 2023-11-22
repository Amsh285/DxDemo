#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"

#include "Windows/Window.h"
#include "Windows/WindowApplication.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Rendering/Direct3dRenderer.h"

namespace dsr
{
	class DsrApplication
	{
	public:
		void Initialize();

		DsrApplication(
			const std::wstring& title,
			const int& x, const int& y,
			const int& width, const int& height);
	protected:
		std::shared_ptr<windows::Window> m_window;
		std::shared_ptr<directX::Direct3dDevice> m_device;
		std::shared_ptr<directX::rendering::Direct3dRenderer> m_renderer;
		std::shared_ptr<windows::WindowApplication> m_windowApplication;

	private:
		class WindowManager
		{
		public:
			void OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent);
			void OnWindowResize(const events::WindowResizedEvent& resizeEvent);

			WindowManager(
				const std::shared_ptr<windows::Window>& window,
				const std::shared_ptr<directX::Direct3dDevice>& device);
			WindowManager(const WindowManager& other) = delete;
			WindowManager& operator=(const WindowManager& other) = delete;
		private:
			std::shared_ptr<windows::Window> m_window;
			std::shared_ptr<directX::Direct3dDevice> m_device;
		};

		std::shared_ptr<WindowManager> m_windowManager;
	};
}
