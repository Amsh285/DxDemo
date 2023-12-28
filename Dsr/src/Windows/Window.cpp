
#include "dsrpch.h"
#include "Window.h"

namespace dsr
{
	namespace windows
	{
		LRESULT Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
		{
			Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

			switch (message)
			{
			case WM_SIZE:
			{
				pWnd->m_data->clientWidth = LOWORD(lParam);
				pWnd->m_data->clientHeight = HIWORD(lParam);

				RECT windowArea = { 0, 0, pWnd->m_data->clientWidth, pWnd->m_data->clientHeight };
				AdjustWindowRect(&windowArea, WS_OVERLAPPEDWINDOW, FALSE);
				pWnd->m_data->width = windowArea.right - windowArea.left;
				pWnd->m_data->height = windowArea.bottom - windowArea.top;

				dsr::events::WindowResizedEvent resizedEvent;
				pWnd->m_windowResizedEmitter.operator()(resizedEvent);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				std::cout << "ka" << std::endl;
				break;
			}
			case WM_MOUSEMOVE:
			{
				std::cout << "mousemove: " << std::endl;
				break;
			}
			case WM_KEYDOWN:
			{
				std::uint8_t keyCode = LOBYTE(wParam);
				std::cout << "Keydown keyCode: " << static_cast<std::uint32_t>(keyCode) << std::endl;

				pWnd->m_keyDownEventEmitter.operator()(dsr::events::KeyDownEvent(keyCode));
				break;
			}
			case WM_KEYUP:
			{
				std::uint8_t keyCode = LOBYTE(wParam);
				std::cout << "-------------------Keyup keyCode: " << static_cast<std::uint32_t>(keyCode) << std::endl;

				pWnd->m_keyUpEventEmitter.operator()(dsr::events::KeyUpEvent(keyCode));
				break;
			}
			case WM_CLOSE:
			{
				dsr::events::WindowCloseEvent closeEvent;
				pWnd->m_windowCloseEventEmitter.operator()(closeEvent);
				break;
			}
			case WM_DESTROY:
			{
				dsr::events::WindowDestroyEvent destroyEvent;
				pWnd->m_windowDestroyEmitter.operator()(destroyEvent);
				return 0;
			}
			default:
				break;
			}

			//std::cout << "message: " << message << std::endl;

			return DefWindowProc(windowHandle, message, wParam, lParam);
		}

		int Window::GetNextClassId()
		{
			static int nextId = 0;
			return ++nextId;
		}

		HWND Window::SetupWindow(const WNDCLASSEX& windowClass)
		{
			if (m_data->useClientHeight)
			{
				RECT windowArea = { 0, 0, m_data->clientWidth, m_data->clientHeight };
				AdjustWindowRect(&windowArea, WS_OVERLAPPEDWINDOW, FALSE);

				m_data->width = windowArea.right - windowArea.left;
				m_data->height = windowArea.bottom - windowArea.top;
			}

			HWND handle = CreateWindowEx(
				NULL,
				m_className.c_str(),
				m_data->title.c_str(),
				WS_OVERLAPPEDWINDOW,
				m_data->x,
				m_data->y,
				m_data->width,
				m_data->height,
				NULL,
				NULL,
				windowClass.hInstance,
				NULL
			);

			if (!handle)
			{
				UnregisterClass(m_className.c_str(), GetModuleHandle(NULL));
				throw dsr::dsr_error("Error Creating window.", GetLastError());
			}

			RECT clientArea;
			GetClientRect(handle, &clientArea);

			m_data->clientWidth = clientArea.right - clientArea.left;
			m_data->clientHeight = clientArea.bottom - clientArea.top;

			return handle;
		}


		Window::Window(const WindowData& data)
		{
			m_className = m_className + std::to_wstring(GetNextClassId());

			m_data = std::make_unique<WindowData>(data);

			WNDCLASSEX windowClass;
			ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = data.style;
			windowClass.lpfnWndProc = Window::WndProc;
			windowClass.hInstance = GetModuleHandle(NULL);
			windowClass.hCursor = data.cursor;
			windowClass.hbrBackground = data.background;
			windowClass.lpszClassName = m_className.c_str();

			WORD register_status = RegisterClassEx(&windowClass);

			if (!register_status)
				throw dsr::dsr_error("Error registering windowclass. Error Code:" + register_status, register_status);

			m_windowHandle = SetupWindow(windowClass);
			assert(m_windowHandle);

			SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		}

		Window::~Window()
		{
			UnregisterClass(m_className.c_str(), GetModuleHandle(NULL));
			DestroyWindow(m_windowHandle);
		}

		void Window::Show()
		{
			ShowWindow(m_windowHandle, m_data->cmdShow);
		}

		void Window::Close()
		{
			CloseWindow(m_windowHandle);
		}
	}
}
