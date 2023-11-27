#pragma once

#include "Events/EventListener.h"
#include "Events/Application/WindowEvents.h"

#include "Windows/Window.h"
#include "Windows/WindowApplication.h"

#include "CameraSystem/Camera.h"

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Rendering/Direct3dRenderer.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"

#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"

namespace dsr
{
	class DsrApplication
	{
	public:
		void Initialize();
		virtual DsrResult Setup();
		void Run();

		DsrApplication(
			const std::wstring& title,
			const int& x, const int& y,
			const int& width, const int& height);
	protected:
		std::shared_ptr<camerasystem::Camera> CreateCamera();
		void SetActiveCamera(const std::shared_ptr<camerasystem::Camera>& camera);

		std::shared_ptr<windows::Window> m_window;
		std::shared_ptr<directX::Direct3dDevice> m_device;
		std::shared_ptr<directX::rendering::Direct3dRenderer> m_renderer;
		std::shared_ptr<windows::WindowApplication> m_windowApplication;

		std::shared_ptr<camerasystem::Camera> m_mainCamera;

		class WindowManager : public events::EventListener
		{
		public:
			void OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent);
			void OnWindowResize(const events::WindowResizedEvent& resizeEvent);

			std::vector<std::shared_ptr<camerasystem::Camera>> Cameras;

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

	private:

	};
}
