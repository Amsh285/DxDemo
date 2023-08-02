#pragma once

#include "WindowData.h"
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
			template<class TEvent>
			using EventRegisterType = dsr::events::EventRegister<dsr::events::EventListener, TEvent>;

			HWND GetWindowHandle() const { return m_windowHandle; }

			EventRegisterType<const dsr::events::WindowCloseEvent&>& GetCloseEventRegister() { return m_windowCloseEventEmitter; }
			EventRegisterType<const dsr::events::WindowDestroyEvent&>& GetDestroyEventRegister() { return m_windowDestroyEmitter; }

			Window(const WindowData& data);
			Window(const Window& other) = delete;
			Window& operator=(const Window& other) = delete;
			~Window();

			void Show();
			void Close();
		private:
			template<class TEvent>
			using EventEmitterType = dsr::events::EventEmitter < dsr::events::EventListener, TEvent>;

			static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

			HWND m_windowHandle;

			std::wstring m_className = L"Window_";
			std::unique_ptr<WindowData> m_data;

			int GetNextClassId();

			EventEmitterType<const dsr::events::WindowCloseEvent&> m_windowCloseEventEmitter;
			EventEmitterType<const dsr::events::WindowDestroyEvent&> m_windowDestroyEmitter;
		};
	}
}
