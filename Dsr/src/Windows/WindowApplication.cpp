
#include "dsrpch.h"
#include "WindowApplication.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Input/WinInput.h"

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

		void WindowApplication::RegisterEvents(std::shared_ptr<dsr::EventDispatcher> dispatcher)
		{
			m_inputSystem->RegisterEvents(dispatcher);
		}

		void WindowApplication::PeekMessages()
		{
			using namespace dsr::input;
			using namespace dsr::inputdevices;
			
			using namespace std::chrono;

			assert(m_device);

			std::shared_ptr<Keyboard> keyboard = m_inputSystem->GetKeyboard();
			std::shared_ptr<Mouse> mouse = m_inputSystem->GetMouse();

			MSG message = { 0 };
			dsr::time::FrameDelta frameDelta;

			while (true)
			{
				dsr::Duration deltaTime = frameDelta.Update();
				m_time->SetDeltaTime(deltaTime);
				
				if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);

					if (message.message == WM_QUIT)
						break;
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				m_device->Clear(0.3f, 0.3f, 0.3f, 1.0f);

				dsr::events::UpdateFrameEvent event(deltaTime);
				m_updateFrameEmitter.operator()(event);

				//sweet
				ImGui::Text("Hello, world %d", 123);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				m_device->SwapBuffers();

				keyboard->PrepareNextFrame();
				mouse->PrepareNextFrame();
			}

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		void WindowApplication::ShutDown()
		{
			PostQuitMessage(0);
		}

		WindowApplication::WindowApplication()
		{
			m_time = std::make_shared<dsr::time::Time>();
			m_inputSystem = std::make_shared<dsr::input::InputSystem>(dsr::windows::CreateKeyMap());
		}
	}
}
