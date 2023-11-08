
#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/EventListeners/WindowEventListener.h"
#include "DirectX/Direct3dDevice.h"
#include "DirectX/Rendering/Direct3dRenderer.h"
#include "DirectX/Shader/Direct3dShader.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"
#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"

#include <iostream>

struct VertexPosColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> LoadContent(const std::shared_ptr<dsr::directX::Direct3dDevice> device)
{
	using namespace dsr;
	using namespace dsr::directX;

	const size_t vertexCount = 8;
	const size_t elementCount = vertexCount * 6;

	VertexPosColor g_Vertices[vertexCount] =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) }  // 7
	};

	float* ptr = reinterpret_cast<float*>(g_Vertices);
	std::vector<float> vertexData(ptr, ptr + elementCount);

	std::vector<uint32_t> indexData =
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector3f("COLOR");

	std::variant<Direct3dVertexBufferf, dsr_error> loadVertexData = SetupVertexBufferf(device, vertexData, indexData, inputLayout);

	if (std::holds_alternative<dsr_error>(loadVertexData))
		return dsr_error::Attach("Error Loading Vertexbuffer: ", std::get<dsr_error>(loadVertexData));

	return std::get<Direct3dVertexBufferf>(loadVertexData);
}

std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> LoadShaderProgram(std::shared_ptr<dsr::directX::Direct3dDevice> device)
{
	using namespace dsr;
	using namespace dsr::directX;

	std::variant<Direct3dShader<ID3D11VertexShader>, dsr_error> loadVertexShader = LoadShaderFromFile<ID3D11VertexShader>(device, L"Assets/VertexShader.hlsl", "vs_5_0");
	std::variant<Direct3dShader<ID3D11PixelShader>, dsr::dsr_error> loadPixelShader = LoadShaderFromFile<ID3D11PixelShader>(device, L"Assets/PixelShader.hlsl", "ps_5_0");

	if (std::holds_alternative<dsr_error>(loadVertexShader))
	{
		dsr_error error = std::get<dsr_error>(loadVertexShader);
		return dsr_error::Attach("Could not load VertexShader. ", error);
	}

	if (std::holds_alternative<dsr_error>(loadPixelShader))
	{
		dsr_error error = std::get<dsr_error>(loadPixelShader);
		return dsr_error::Attach("Could not load PixelShader. ", error);
	}

	Direct3dShader<ID3D11VertexShader> vertexShader = std::get<Direct3dShader<ID3D11VertexShader>>(loadVertexShader);
	Direct3dShader<ID3D11PixelShader> pixelShader = std::get<Direct3dShader<ID3D11PixelShader>>(loadPixelShader);

	Direct3dShaderInputLayout vertexShaderInputLayout;
	/*vertexShaderInputLayout.AddVector3f("POSITION");
	vertexShaderInputLayout.AddVector3f("COLOR");*/

	// Todo: Testen ob beides geht
	vertexShaderInputLayout.AddVector3f("POSITION", 0 , 0, offsetof(VertexPosColor, Position));
	vertexShaderInputLayout.AddVector3f("COLOR", 0, 0, offsetof(VertexPosColor, Color));

	std::variant<Direct3dShaderProgram, dsr_error> loadShaderProgram = CreateShaderProgram(
		device, vertexShader, pixelShader, vertexShaderInputLayout);

	if (std::holds_alternative<dsr_error>(loadShaderProgram))
	{
		dsr_error error = std::get<dsr_error>(loadShaderProgram);
		return dsr_error::Attach("Could not load Shaderprogram. ", error);
	}

	return std::get<Direct3dShaderProgram>(loadShaderProgram);
}

int main(int argc, char* argv[])
{
	using namespace dsr;
	using namespace dsr::directX;
	using namespace dsr::windows;

	try
	{
		WindowData data(L"Basic renderer", 100, 100, 1280, 768);

		std::shared_ptr<Window> window =
			std::make_shared<Window>(data);

		std::shared_ptr<WindowEventListener> destroyListener = std::make_shared<WindowEventListener>();
		window->GetDestroyEventRegister().Hook(destroyListener, &WindowEventListener::OnWindowDestroy);

		std::shared_ptr<Direct3dDevice> device = Direct3dDevice::Create(window);
		std::shared_ptr<rendering::Direct3dRenderer> renderer = std::make_shared<rendering::Direct3dRenderer>(device);

		std::variant<Direct3dVertexBufferf, dsr_error> loadContent = LoadContent(device);

		if (std::holds_alternative<dsr_error>(loadContent))
		{
			dsr_error& error = std::get<dsr_error>(loadContent);
			std::cout << "error loading content: " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		std::variant<Direct3dShaderProgram, dsr_error> loadShaderProgram = LoadShaderProgram(device);

		if (std::holds_alternative<dsr_error>(loadShaderProgram))
		{
			dsr_error error = std::get<dsr_error>(loadShaderProgram);
			std::cout << "error loading shader program: " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		Direct3dVertexBufferf vertexBuffer = std::get<Direct3dVertexBufferf>(loadContent);
		Direct3dShaderProgram shaderProgram = std::get<Direct3dShaderProgram>(loadShaderProgram);

		window->Show();

		std::shared_ptr<WindowApplication> app = WindowApplication::Get();

		app->GetUpdateFrameEventRegister().Hook(renderer, &rendering::Direct3dRenderer::OnUpdate);
		app->PeekMessages();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
