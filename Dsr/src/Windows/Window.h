#pragma once

#include "WindowData.h"
#include "MouseEventTracker.h"
#include "ErrorHandling/dsr_error.h"
#include "Events/EventEmitter.h"
#include "Events/EventListener.h"
#include "Events/IEvent.h"
#include "Events/Application/WindowEvents.h"
#include "Events/Application/InputEvents/KeyboardEvents.h"
#include "Events/Application/InputEvents/MouseEvents.h"

namespace dsr
{
	namespace windows
	{
		class Window
		{
		public:
			HWND GetWindowHandle() const { return m_windowHandle; }

			// maybe better to return and store them as shared pointers. returning references is not very safe.
			EventRegisterType<const dsr::events::WindowCloseEvent&>& GetCloseEventRegister() { return m_windowCloseEventEmitter; }
			EventRegisterType<const dsr::events::WindowDestroyEvent&>& GetDestroyEventRegister() { return m_windowDestroyEmitter; }
			EventRegisterType<const dsr::events::WindowResizedEvent&>& GetResizedEventRegister() { return m_windowResizedEmitter; }
			EventRegisterType<const dsr::events::AquiredFocusEvent&>& GetAquiredFocusEventRegister() { return m_aquiredFocusEvent; }
			EventRegisterType<const dsr::events::LooseFocusEvent&>& GetLooseFocusEventRegister() { return m_looseFocusEvent; }

			EventRegisterType<const dsr::events::KeyDownEvent&>& GetKeyDownEventRegister() { return m_keyDownEventEmitter; }
			EventRegisterType<const dsr::events::KeyUpEvent&>& GetKeyUpEventRegister() { return m_keyUpEventEmitter; }

			EventRegisterType<const dsr::events::MouseDownEvent&>& GetMouseDownEventRegister() { return m_mouseDownEventEmitter; }
			EventRegisterType<const dsr::events::MouseUpEvent&>& GetMouseUpEventRegister() { return m_mouseUpEventEmitter; }
			EventRegisterType<const dsr::events::MouseWheelEvent&>& GetMouseWheelEventRegister() { return m_mouseWheelEventEmitter; }
			EventRegisterType<const dsr::events::MouseMoveEvent&>& GetMouseMoveEventRegister() { return m_mouseMoveEventEmitter; }
			EventRegisterType<const dsr::events::MouseHoverEvent&>& GetMouseHoverEventRegister() { return m_mouseHoverEventEmitter; }
			EventRegisterType<const dsr::events::MouseLeaveEvent&>& GetMouseLeaveEventRegister() { return m_mouseLeaveEventEmitter; }

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
			std::shared_ptr<MouseEventTracker> m_mouseEventTracker;

			int GetNextClassId();
			HWND SetupWindow(const WNDCLASSEX& windowClass);

			EventEmitterType<const dsr::events::WindowCloseEvent&> m_windowCloseEventEmitter;
			EventEmitterType<const dsr::events::WindowDestroyEvent&> m_windowDestroyEmitter;
			EventEmitterType<const dsr::events::WindowResizedEvent&> m_windowResizedEmitter;
			EventEmitterType<const dsr::events::AquiredFocusEvent&> m_aquiredFocusEvent;
			EventEmitterType<const dsr::events::LooseFocusEvent&> m_looseFocusEvent;

			EventEmitterType<const dsr::events::KeyDownEvent&> m_keyDownEventEmitter;
			EventEmitterType<const dsr::events::KeyUpEvent&> m_keyUpEventEmitter;

			EventEmitterType<const dsr::events::MouseDownEvent&> m_mouseDownEventEmitter;
			EventEmitterType<const dsr::events::MouseUpEvent&> m_mouseUpEventEmitter;
			EventEmitterType<const dsr::events::MouseWheelEvent&> m_mouseWheelEventEmitter;
			EventEmitterType<const dsr::events::MouseMoveEvent&> m_mouseMoveEventEmitter;
			EventEmitterType<const dsr::events::MouseHoverEvent&> m_mouseHoverEventEmitter;
			EventEmitterType<const dsr::events::MouseLeaveEvent&> m_mouseLeaveEventEmitter;
		};
	}
}
