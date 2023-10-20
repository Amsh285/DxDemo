
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
	try
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

		std::variant<dsr::directX::Direct3dShader<ID3D11VertexShader>, dsr::dsr_error> loadVertexShader =
			dsr::directX::LoadShaderFromFile<ID3D11VertexShader>(device, L"Assets/VertexShader.hlsl", "vs_5_0");

		std::variant<dsr::directX::Direct3dShader<ID3D11PixelShader>, dsr::dsr_error> loadPixelShader =
			dsr::directX::LoadShaderFromFile<ID3D11PixelShader>(device, L"Assets/PixelShader.hlsl", "ps_5_0");

		if (std::holds_alternative<dsr::dsr_error>(loadVertexShader))
		{
			dsr::dsr_error error = std::get<dsr::dsr_error>(loadVertexShader);
			std::cout << "Could not load VertexShader. " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		if (std::holds_alternative<dsr::dsr_error>(loadPixelShader))
		{
			dsr::dsr_error error = std::get<dsr::dsr_error>(loadPixelShader);
			std::cout << "Could not load PixelShader. " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		dsr::directX::Direct3dShader<ID3D11VertexShader> vertexShader =
			std::get<dsr::directX::Direct3dShader<ID3D11VertexShader>>(loadVertexShader);

		dsr::directX::Direct3dShader<ID3D11PixelShader> pixelShader =
			std::get<dsr::directX::Direct3dShader<ID3D11PixelShader>>(loadPixelShader);

		window->Show();

		std::shared_ptr<dsr::windows::WindowApplication> app = dsr::windows::WindowApplication
			::Get();

		app->GetUpdateFrameEventRegister().Hook(renderer, &dsr::directX::Direct3dRenderer::OnUpdate);
		app->PeekMessages();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
