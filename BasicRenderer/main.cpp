
#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/EventListeners/WindowEventListener.h"
#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dRenderer.h"
#include "DirectX/Direct3dShader.h"

#include <iostream>

int main(int argc, char* argv[])
{
	dsr::windows::WindowData data(L"Basic renderer", 100, 100, 1280, 768);

	std::shared_ptr<dsr::windows::Window> window =
		std::make_shared<dsr::windows::Window>(data);

	std::shared_ptr<dsr::windows::WindowEventListener> destroyListener = std::make_shared<dsr::windows::WindowEventListener>();
	window->GetDestroyEventRegister().Hook(destroyListener, &dsr::windows::WindowEventListener::OnWindowDestroy);

	std::shared_ptr<dsr::directX::Direct3dDevice> device =
		dsr::directX::Direct3dDevice::Create(window);

	std::shared_ptr<dsr::directX::Direct3dRenderer> renderer =
		std::make_shared<dsr::directX::Direct3dRenderer>(device);

	dsr::directX::LoadShaderFromFile<ID3D11VertexShader>(device, L"", "");

	window->Show();

	std::shared_ptr<dsr::windows::WindowApplication> app = dsr::windows::WindowApplication
		::Get();

	app->GetUpdateFrameEventRegister().Hook(renderer, &dsr::directX::Direct3dRenderer::OnUpdate);
	app->PeekMessages();
}
