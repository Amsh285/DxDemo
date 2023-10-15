
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
			case WM_LBUTTONDOWN:
			{
				std::cout << "ka" << std::endl;
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
				throw std::runtime_error("Error registering windowclass. Error Code:" + register_status);

			m_windowHandle = SetupWindow(windowClass);

			std::cout << "Width: " << m_data->width << std::endl;
			std::cout << "Height: " << m_data->height << std::endl;

			RECT rec;
			GetClientRect(m_windowHandle, &rec);
			std::cout << "ClientWidth: " << m_data->clientWidth << std::endl;
			std::cout << "ClientHeight: " << m_data->clientHeight << std::endl;

			if (!m_windowHandle)
			{
				UnregisterClass(m_className.c_str(), GetModuleHandle(NULL));
				throw std::runtime_error("Error Creating window.");
			}

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
