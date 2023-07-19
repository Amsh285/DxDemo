
#include "dsrpch.h"
#include "Windows/Window.h"

#include <iostream>

//linker/system/subsystem -> Windows
//int WINAPI WinMain(
//    HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPSTR lpCmdLine,
//    int nCmdShow)
//{
//    dsr::windows::WindowData data(L"DemoWindow", L"DemoWindow");
//    dsr::windows::Window window(data);
//    window.Show();
//}

int main(int argc, char* argv[])
{
    std::cout << "hello window" << std::endl;

    dsr::windows::WindowData data(L"DemoWindow");
    dsr::windows::Window window(data);
    window.Show();
}
