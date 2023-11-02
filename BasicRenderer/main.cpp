
#include "dsrpch.h"
#include "Windows/Window.h"
#include "Windows/WindowApplication.h"
#include "Windows/EventListeners/WindowEventListener.h"
#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dRenderer.h"
#include "DirectX/Direct3dShader.h"
#include "DirectX/Direct3dDeviceShaderExtensions.h"
#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Direct3dShaderProgram.h"

#include <iostream>

//enum ConstantBuffer
//{
//	CB_Application,
//	CB_Frame,
//	CB_Object,
//	NumConstantBuffers
//};
//
//ID3D11Buffer* g_d3dConstantBuffers[NumConstantBuffers];

struct VertexPosColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> LoadContent(const std::shared_ptr<dsr::directX::Direct3dDevice> device)
{
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

	dsr::directX::Direct3dShaderInputLayout inputLayout;
	inputLayout.AddVector3f("POSITION");
	inputLayout.AddVector3f("COLOR");

	std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> loadVertexData = dsr::directX::LoadVertexBufferf(device, vertexData, indexData, inputLayout);

	if (std::holds_alternative<dsr::dsr_error>(loadVertexData))
		return dsr::dsr_error::Attach("Error Loading Vertexbuffer: ", std::get<dsr::dsr_error>(loadVertexData));

	return std::get<dsr::directX::Direct3dVertexBufferf>(loadVertexData);
}

std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> LoadShaderProgram(std::shared_ptr<dsr::directX::Direct3dDevice> device)
{
	std::variant<dsr::directX::Direct3dShader<ID3D11VertexShader>, dsr::dsr_error> loadVertexShader =
		dsr::directX::LoadShaderFromFile<ID3D11VertexShader>(device, L"Assets/VertexShader.hlsl", "vs_5_0");

	std::variant<dsr::directX::Direct3dShader<ID3D11PixelShader>, dsr::dsr_error> loadPixelShader =
		dsr::directX::LoadShaderFromFile<ID3D11PixelShader>(device, L"Assets/PixelShader.hlsl", "ps_5_0");

	if (std::holds_alternative<dsr::dsr_error>(loadVertexShader))
	{
		dsr::dsr_error error = std::get<dsr::dsr_error>(loadVertexShader);
		return dsr::dsr_error::Attach("Could not load VertexShader. ", error);
	}

	if (std::holds_alternative<dsr::dsr_error>(loadPixelShader))
	{
		dsr::dsr_error error = std::get<dsr::dsr_error>(loadPixelShader);
		return dsr::dsr_error::Attach("Could not load PixelShader. ", error);
	}

	dsr::directX::Direct3dShader<ID3D11VertexShader> vertexShader =
		std::get<dsr::directX::Direct3dShader<ID3D11VertexShader>>(loadVertexShader);

	dsr::directX::Direct3dShader<ID3D11PixelShader> pixelShader =
		std::get<dsr::directX::Direct3dShader<ID3D11PixelShader>>(loadPixelShader);

	dsr::directX::Direct3dShaderInputLayout vertexShaderInputLayout;
	/*vertexShaderInputLayout.AddVector3f("POSITION");
	vertexShaderInputLayout.AddVector3f("COLOR");*/

	// kann sein das man das machen muss weil der input ein struct array ist.
	// Todo: Testen ob beides geht
	vertexShaderInputLayout.AddVector3f("POSITION", 0 , 0, offsetof(VertexPosColor, Position));
	vertexShaderInputLayout.AddVector3f("COLOR", 0, 0, offsetof(VertexPosColor, Color));

	std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> loadShaderProgram = dsr::directX::CreateShaderProgram(
		device, vertexShader, pixelShader, vertexShaderInputLayout);

	if (std::holds_alternative<dsr::dsr_error>(loadShaderProgram))
	{
		dsr::dsr_error error = std::get<dsr::dsr_error>(loadShaderProgram);
		return dsr::dsr_error::Attach("Could not load Shaderprogram. ", error);
	}

	return std::get<dsr::directX::Direct3dShaderProgram>(loadShaderProgram);
}

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

		std::variant<dsr::directX::Direct3dVertexBufferf, dsr::dsr_error> loadContent = LoadContent(device);

		if (std::holds_alternative<dsr::dsr_error>(loadContent))
		{
			dsr::dsr_error& error = std::get<dsr::dsr_error>(loadContent);
			std::cout << "error loading content: " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		std::variant<dsr::directX::Direct3dShaderProgram, dsr::dsr_error> loadShaderProgram = LoadShaderProgram(device);

		if (std::holds_alternative<dsr::dsr_error>(loadShaderProgram))
		{
			dsr::dsr_error error = std::get<dsr::dsr_error>(loadShaderProgram);
			std::cout << "error loading shader program: " << error.what() << std::endl;
			return EXIT_FAILURE;
		}

		dsr::directX::Direct3dVertexBufferf vertexBuffer = std::get<dsr::directX::Direct3dVertexBufferf>(loadContent);
		dsr::directX::Direct3dShaderProgram shaderProgram = std::get<dsr::directX::Direct3dShaderProgram>(loadShaderProgram);

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
