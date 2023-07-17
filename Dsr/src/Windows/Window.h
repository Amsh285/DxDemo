#pragma once

namespace dsr
{
	namespace windows
	{
		struct WindowData
		{
			HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
			HBRUSH background = (HBRUSH)COLOR_WINDOW;
			UINT style = CS_HREDRAW | CS_VREDRAW;

			std::wstring className, title;
			int x = 100, y = 100;
			int clientWidth = 500, clientHeight = 400;
			int cmdShow = SW_SHOWNORMAL;

			WindowData(const std::wstring& className, const std::wstring& title)
			{
				this->className = className;
				this->title = title;
			}
		};

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
			std::unique_ptr<WindowData> m_data;
		};
	}
}
