
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

			ComPtr<ID3D11Device> d3device;
			ComPtr<ID3D11DeviceContext> d3deviceContext;
			ComPtr<IDXGISwapChain> d3swapChain;

			HRESULT result = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				NULL,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				&swapChainData,
				&d3swapChain,
				&d3device,
				NULL,
				&d3deviceContext
			);

			if (FAILED(result))
				throw createdirecd3ddevice_error("Failed to create Device and Swapchain.", result);

			std::shared_ptr<Direct3dDevice> ptr(new Direct3dDevice());
			ptr->m_device = d3device;
			ptr->m_deviceContext = d3deviceContext;
			ptr->m_swapChain = d3swapChain;
			ptr->m_window = window;

			return ptr;
		}
	}
}
