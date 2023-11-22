#pragma once

#include "WindowData.h"
#include "ErrorHandling/dsr_error.h"
#include "Events/EventEmitter.h"
#include "Events/EventListener.h"
#include "Events/IEvent.h"
#include "Events/Application/WindowEvents.h"

namespace dsr
{
	namespace windows
	{
		class Window
		{
		public:
			HWND GetWindowHandle() const { return m_windowHandle; }

			EventRegisterType<const dsr::events::WindowCloseEvent&>& GetCloseEventRegister() { return m_windowCloseEventEmitter; }
			EventRegisterType<const dsr::events::WindowDestroyEvent&>& GetDestroyEventRegister() { return m_windowDestroyEmitter; }
			EventRegisterType<const dsr::events::WindowResizedEvent&>& GetResizedEventRegister() { return m_windowResizedEmitter; }

			int GetClientWidth() const { return m_data->clientWidth; }
			int GetClientHeight() const { return m_data->clientHeight; }

			Window(const WindowData& data);
			Window(const Window& other) = delete;
			Window& operator=(const Window& other) = delete;
			~Window();

			void Show();
			void Close();
		private:
			static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

			HWND m_windowHandle;
			std::wstring m_className = L"Window_";
			std::unique_ptr<WindowData> m_data;

			int GetNextClassId();
			HWND SetupWindow(const WNDCLASSEX& windowClass);

			EventEmitterType<const dsr::events::WindowCloseEvent&> m_windowCloseEventEmitter;
			EventEmitterType<const dsr::events::WindowDestroyEvent&> m_windowDestroyEmitter;
			EventEmitterType<const dsr::events::WindowResizedEvent&> m_windowResizedEmitter;
		};
	}
}
