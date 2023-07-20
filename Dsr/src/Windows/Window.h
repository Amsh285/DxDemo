#pragma once

#include "WindowData.h"

namespace dsr
{
	namespace windows
	{
		class Window
		{
		public:
			HWND GetWindowHandle() const { return m_windowHandle; }

			Window(const WindowData& data);
			Window(const Window& other) = delete;
			Window& operator=(const Window& other) = delete;
			~Window();

			void Show();
		private:
			static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

			HWND m_windowHandle;

			std::wstring m_className = L"Window_";
			std::unique_ptr<WindowData> m_data;

			int GetNextClassId();
		};
	}
}
