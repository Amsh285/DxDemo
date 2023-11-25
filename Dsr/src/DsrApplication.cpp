#include "dsrpch.h"
#include "DsrApplication.h"

namespace dsr
{
	void DsrApplication::WindowManager::OnWindowDestroy(const events::WindowDestroyEvent& destroyEvent)
	{
		using namespace dsr::windows;

		std::shared_ptr<WindowApplication> application = WindowApplication::Get();
		application->ShutDown();
	}

	void DsrApplication::WindowManager::OnWindowResize(const events::WindowResizedEvent& resizeEvent)
	{
		using namespace dsr;
		using namespace dsr::directX;

		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

		int clientWidth = m_window->GetClientWidth();
		int clientHeight = m_window->GetClientHeight();

		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.Width = static_cast<float>(clientWidth);
		viewPort.Height = static_cast<float>(clientHeight);
		m_device->SetViewports(1, &viewPort);

		

		float aspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);
		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(45.0f),
			aspectRatio,
			0.1f, 100.0f);

		std::cout << "aspect ratio: " << aspectRatio << std::endl;

		for (Direct3dShaderProgram program : ShaderPrograms)
		{
			DsrResult setProjectionMatrixResult = SetConstantBuffer(m_device, program.VertexShader, 0, projectionMatrix);
		}
	}

	DsrApplication::WindowManager::WindowManager(
		const std::shared_ptr<windows::Window>& window,
		const std::shared_ptr<directX::Direct3dDevice>& device)
		: m_window(window), m_device(device)
	{
	}

	void DsrApplication::Initialize()
	{
		m_device = directX::Direct3dDevice::Create(m_window);
		m_renderer = std::make_shared<directX::rendering::Direct3dRenderer>(m_device);

		m_windowManager = std::make_shared<WindowManager>(m_window, m_device);

		m_window->GetDestroyEventRegister().Hook(m_windowManager, &DsrApplication::WindowManager::OnWindowDestroy);
		m_window->GetResizedEventRegister().Hook(m_windowManager, &DsrApplication::WindowManager::OnWindowResize);
		m_windowApplication->GetUpdateFrameEventRegister().Hook(m_renderer, &directX::rendering::Direct3dRenderer::OnUpdate);
	}

	void DsrApplication::Run()
	{
		m_window->Show();
		m_windowApplication->PeekMessages();
	}

	DsrApplication::DsrApplication(
		const std::wstring& title,
		const int& x, const int& y,
		const int& width, const int& height)
		: m_window(std::make_shared<windows::Window>(windows::WindowData(title, x, y, width, height))),
		m_windowApplication(windows::WindowApplication::Get())
	{
	}
}
