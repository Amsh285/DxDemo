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

			std::wstring title;
			int x = 100, y = 100;
			int clientWidth = 500, clientHeight = 400;
			int cmdShow = SW_SHOWNORMAL;

			WindowData(const std::wstring& title)
			{
				this->title = title;
			}

			WindowData(const std::wstring& title, const int& x, const int& y, const int& clientWidth, const int& clientHeight)
			{
				this->title = title;
				this->x = x;
				this->y = y;
				this->clientWidth = clientWidth;
				this->clientHeight = clientHeight;
			}
		};
	}
}
