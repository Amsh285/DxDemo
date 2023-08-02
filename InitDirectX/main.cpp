
//$(SolutionDir)Dsr\src

#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/EventListeners/WindowEventListener.h"
#include "DirectX/Direct3dDevice.h"

#include <iostream>

int main(int argc, char* argv[])
{
    dsr::windows::WindowData data(L"DemoWindow", 100, 100, 500, 300);
    std::shared_ptr<dsr::windows::Window> window =
        std::make_shared<dsr::windows::Window>(data);
    
    std::shared_ptr<dsr::windows::WindowEventListener> destroyListener = std::make_shared<dsr::windows::WindowEventListener>();
    window->GetDestroyEventRegister().Hook(destroyListener, &dsr::windows::WindowEventListener::OnWindowDestroy);

    std::shared_ptr<dsr::directX::Direct3dDevice> device =
        dsr::directX::Direct3dDevice::Create(window);

    window->Show();

    std::shared_ptr<dsr::windows::WindowApplication> app = dsr::windows::WindowApplication
        ::Get();

    app->PeekMessages();
}
