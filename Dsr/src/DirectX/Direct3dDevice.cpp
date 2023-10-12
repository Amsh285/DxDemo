
#include "dsrpch.h"
#include "Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		Direct3dDevice::DevicePtr Direct3dDevice::Create(const WindowPtr& window)
		{
			DXGI_SWAP_CHAIN_DESC swapChainData;
			ZeroMemory(&swapChainData, sizeof(DXGI_SWAP_CHAIN_DESC));

			swapChainData.BufferCount = 1;
			swapChainData.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainData.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainData.OutputWindow = window->GetWindowHandle();
			swapChainData.SampleDesc.Count = 4;
			swapChainData.Windowed = TRUE;

			std::shared_ptr<Direct3dDevice> device(new Direct3dDevice());
			device->m_window = window;

			HRESULT result = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				NULL,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				&swapChainData,
				&device->m_swapChain,
				&device->m_device,
				NULL,
				&device->m_deviceContext
			);

			if (FAILED(result))
				throw createdirecd3ddevice_error("Failed to create Device and Swapchain.", result);

			// set BackBuffer as Rendertarget
			ID3D11Texture2D* pBackBuffer;
			HRESULT getBackBufferFromSwapChainResult = device->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

			if (FAILED(getBackBufferFromSwapChainResult) || !pBackBuffer)
				throw createdirecd3ddevice_error("Failed to access the BackBuffer of the Swapchain.", getBackBufferFromSwapChainResult);

			// rendertargetview null.. error is probably here...
			HRESULT createRenderTargetViewResult =  device->m_device->CreateRenderTargetView(pBackBuffer, NULL, &device->m_renderTargetView);
			pBackBuffer->Release();

			if (FAILED(createRenderTargetViewResult))
				throw createdirecd3ddevice_error("Failed to Create the RenderTargetView.", createRenderTargetViewResult);

			// set render target for deviceContext.
			device->m_deviceContext->OMSetRenderTargets(1, &device->m_renderTargetView, NULL);
			
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = device->m_window->GetClientWidth();
			viewport.Height = device->m_window->GetClientHeight();

			device->m_deviceContext->RSSetViewports(1, &viewport);

			return device;
		}

		void Direct3dDevice::Clear()
		{
			//Todo: add parameter
			float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
			m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		}

		void Direct3dDevice::SwapBuffers()
		{
			m_swapChain->Present(0, 0);
		}
	}
}
