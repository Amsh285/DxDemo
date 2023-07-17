
//$(SolutionDir)Dsr\src

#include "dsrpch.h"
#include "Windows/Window.h"
#include "DirectX/Direct3dDevice.h"

#include <iostream>

int main(int argc, char* argv[])
{
    dsr::windows::WindowData data(L"DemoWindow", L"DemoWindow");
    std::shared_ptr<dsr::windows::Window> window =
        std::make_shared<dsr::windows::Window>(data);
    
    std::shared_ptr<dsr::directX::Direct3dDevice> device =
        dsr::directX::Direct3dDevice::Create(window);

    window->Show();
}
