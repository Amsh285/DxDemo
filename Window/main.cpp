
#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/EventListeners/WindowEventListener.h"

class WindowCloseListener : public dsr::events::EventListener
{
public:
    void OnWindowClosed(const dsr::events::WindowCloseEvent& e)
    {
        std::cout << "closing window..." << std::endl;
    }
};

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

    std::shared_ptr<WindowCloseListener> closeListener = std::make_shared<WindowCloseListener>();
    std::shared_ptr<dsr::windows::WindowEventListener> destroyListener = std::make_shared<dsr::windows::WindowEventListener>();

    window.GetCloseEventRegister().Hook(closeListener, &WindowCloseListener::OnWindowClosed);
    window.GetDestroyEventRegister().Hook(destroyListener, &dsr::windows::WindowEventListener::OnWindowDestroy);

    dsr::windows::WindowData d2(L"yeeeee", 800, 100, 200, 500);
    dsr::windows::Window w2(d2);
    w2.Show();

    std::shared_ptr<dsr::windows::WindowApplication> app = dsr::windows::WindowApplication
        ::Get();

    app->PeekMessages();
}
