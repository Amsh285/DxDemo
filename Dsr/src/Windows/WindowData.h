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

			int x = 0, y = 0;
			int width = 0, height = 0;
			int clientWidth = 0, clientHeight = 0;
			bool useClientHeight;

			int cmdShow = SW_SHOWNORMAL;

			WindowData(const std::wstring& title)
				: WindowData(title, 100, 100, 500, 400)
			{
			}

			WindowData(const std::wstring& title, const int& x, const int& y, const int& width, const int& height, const bool& useClientHeight = true)
			{
				this->title = title;
				this->x = x;
				this->y = y;
				this->useClientHeight = useClientHeight;

				if (useClientHeight)
				{
					this->clientWidth = width;
					this->clientHeight = height;
				}
				else
				{
					this->width = width;
					this->height = height;
				}
			}
		};
	}
}
