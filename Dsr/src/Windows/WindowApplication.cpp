
#include "dsrpch.h"
#include "WindowApplication.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace dsr
{
	namespace windows
	{
		std::shared_ptr<WindowApplication> WindowApplication::m_instance;
		std::mutex WindowApplication::m_appMutex;

		std::shared_ptr<WindowApplication> WindowApplication::Get()
		{
			std::lock_guard<std::mutex> guard(m_appMutex);

			if (!m_instance)
				m_instance = std::shared_ptr<WindowApplication>(new WindowApplication());

			return m_instance;
		}

		void WindowApplication::PeekMessages()
		{
			using namespace std::chrono;

			MSG message = { 0 };
			dsr::time::FrameDelta frameDelta;

			while (true)
			{
				if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);

					if (message.message == WM_QUIT)
						break;
				}

				dsr::Duration deltaTime = frameDelta.Update();

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				//sweet
				//ImGui::Text("Hello, world %d", 123);

				dsr::events::PrepareUpdateFrameEvent prepareEvent(deltaTime);
				m_prepareUpdateFrameEmitter.operator()(prepareEvent);

				dsr::events::UpdateFrameEvent event(deltaTime);
				m_updateFrameEmitter.operator()(event);

				dsr::events::UpdateFrameFinishedEvent updateFinishedEvent(deltaTime);
				m_updateFrameFinishedEmitter.operator()(updateFinishedEvent);
			}

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		void WindowApplication::ShutDown()
		{
			PostQuitMessage(0);
		}
	}
}
