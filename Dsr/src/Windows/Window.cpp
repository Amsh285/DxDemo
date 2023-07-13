
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
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			default:
				break;
			}

			return DefWindowProc(windowHandle, message, wParam, lParam);
		}

		Window::Window(const WindowData& data)
		{
			m_data = std::make_unique<WindowData>(data);

			WNDCLASSEX windowClass;
			ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = data.style;
			windowClass.lpfnWndProc = Window::WndProc;
			windowClass.hInstance = GetModuleHandle(NULL);
			windowClass.hCursor = data.cursor;
			windowClass.hbrBackground = data.background;
			windowClass.lpszClassName = data.className.c_str();

			RECT cr = { 0, 0, data.clientWidth, data.clientHeight };
			AdjustWindowRect(&cr, WS_OVERLAPPEDWINDOW, FALSE);

			WORD register_status = RegisterClassEx(&windowClass);

			if (!register_status)
				throw std::runtime_error("Error registering windowclass. Error Code:" + register_status);

			m_windowHandle = CreateWindowEx(
				NULL,
				data.className.c_str(),
				data.title.c_str(),
				WS_OVERLAPPEDWINDOW,
				data.x,
				data.y,
				cr.right - cr.left,
				cr.bottom - cr.top,
				NULL,
				NULL,
				windowClass.hInstance,
				NULL
			);

			if (!m_windowHandle)
			{
				UnregisterClass(m_data->className.c_str(), GetModuleHandle(NULL));
				throw std::runtime_error("Error Creating window.");
			}

			SetWindowLongPtr(m_windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		}

		Window::~Window()
		{
			UnregisterClass(m_data->className.c_str(), GetModuleHandle(NULL));
			DestroyWindow(m_windowHandle);
		}

		void Window::Show()
		{
			ShowWindow(m_windowHandle, m_data->cmdShow);

			MSG message = { 0 };

			while (true)
			{
				if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);

					if (message.message == WM_QUIT)
						break;
				}
				else
				{

				}
			}
		}
	}
}
