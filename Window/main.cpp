
#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"


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

    dsr::windows::WindowData data(L"DemoWindow", 100, 100, 500, 500);
    dsr::windows::Window window(data);
    window.Show();

    dsr::windows::WindowData d2(L"yeeeee", 800, 100, 200, 500);
    dsr::windows::Window w2(d2);
    w2.Show();

    std::shared_ptr<dsr::windows::WindowApplication> app = dsr::windows::WindowApplication
        ::Get();

    app->PeekMessages();
}
