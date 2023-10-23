
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
			swapChainData.SampleDesc.Quality = 0;
			swapChainData.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainData.Windowed = TRUE;

			std::shared_ptr<Direct3dDevice> device(new Direct3dDevice());
			device->m_window = window;

			UINT createDeviceAndSwapChainFlags = 0;
#if _DEBUG
			createDeviceAndSwapChainFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

			D3D_FEATURE_LEVEL featureLevel;

			HRESULT hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				createDeviceAndSwapChainFlags,
				device->m_requestedFeatureLevels.data(),
				device->m_requestedFeatureLevels.size(),
				D3D11_SDK_VERSION,
				&swapChainData,
				&device->m_swapChain,
				&device->m_device,
				&featureLevel,
				&device->m_deviceContext
			);

			if (FAILED(hr))
				throw dsr_error("Failed to create Device and Swapchain.", hr);

			device->m_featureLevel = std::make_unique<D3D_FEATURE_LEVEL>(featureLevel);

#pragma region setup renderTargetView
			// has to be setup like this. direct member access will result in nullptr.
			ID3D11RenderTargetView* renderTargetView;
			ID3D11Texture2D* pBackBuffer;
			HRESULT getBackBufferFromSwapChainResult = device->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

			if (FAILED(getBackBufferFromSwapChainResult) || !pBackBuffer)
				throw dsr_error("Failed to access the BackBuffer of the Swapchain.", getBackBufferFromSwapChainResult);

			HRESULT createRenderTargetViewResult = device->m_device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
			pBackBuffer->Release();

			if (FAILED(createRenderTargetViewResult))
				throw dsr_error("Failed to Create the RenderTargetView.", createRenderTargetViewResult);

			device->m_renderTargetView = renderTargetView;
#pragma endregion

#pragma region setup depth and stencil buffer
			D3D11_TEXTURE2D_DESC depthStencilBufferDescrition;
			ZeroMemory(&depthStencilBufferDescrition, sizeof(D3D11_TEXTURE2D_DESC));

			depthStencilBufferDescrition.ArraySize = 1;
			depthStencilBufferDescrition.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilBufferDescrition.CPUAccessFlags = 0;
			depthStencilBufferDescrition.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilBufferDescrition.Width = device->m_window->GetClientWidth();
			depthStencilBufferDescrition.Height = device->m_window->GetClientHeight();
			depthStencilBufferDescrition.MipLevels = 1;
			depthStencilBufferDescrition.SampleDesc.Count = 4;
			depthStencilBufferDescrition.SampleDesc.Quality = 0;
			depthStencilBufferDescrition.Usage = D3D11_USAGE_DEFAULT;

			ID3D11Texture2D* depthStencilBuffer;
			HRESULT createDepthStenciBufferResult = device->m_device->CreateTexture2D(&depthStencilBufferDescrition, nullptr, &depthStencilBuffer);

			if (FAILED(createDepthStenciBufferResult))
				throw dsr_error("Failed to Create the DepthStencilBuffer.", createDepthStenciBufferResult);

			HRESULT createDepthStencilViewResult = device->m_device->CreateDepthStencilView(depthStencilBuffer, nullptr, &device->m_depthStencilView);
			depthStencilBuffer->Release();

			if (FAILED(createDepthStencilViewResult))
				throw dsr_error("Failed to Create the DepthStencilView.", createDepthStencilViewResult);
#pragma endregion

#pragma region setup depth and stencil testing options
			D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
			ZeroMemory(&depthStencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));

			depthStencilDescription.DepthEnable = TRUE;
			depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
			depthStencilDescription.StencilEnable = FALSE;

			HRESULT createDepthStencilStateResult = device->m_device->CreateDepthStencilState(&depthStencilDescription, &device->m_depthStencilState);
#pragma endregion

#pragma region setup rasterizer options
			D3D11_RASTERIZER_DESC rasterizerDescription;
			ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

			rasterizerDescription.AntialiasedLineEnable = FALSE;
			rasterizerDescription.CullMode = D3D11_CULL_BACK;
			rasterizerDescription.DepthBias = 0;
			rasterizerDescription.DepthBiasClamp = 0.0f;
			rasterizerDescription.DepthClipEnable = TRUE;
			rasterizerDescription.FillMode = D3D11_FILL_SOLID;
			rasterizerDescription.FrontCounterClockwise = FALSE;
			rasterizerDescription.MultisampleEnable = FALSE;
			rasterizerDescription.ScissorEnable = FALSE;
			rasterizerDescription.SlopeScaledDepthBias = 0.0f;

			HRESULT createRasterizerStateResult = device->m_device->CreateRasterizerState(&rasterizerDescription, &device->m_rasterizerState);

			if (FAILED(createRasterizerStateResult))
				throw dsr_error("Failed to Create the RasterizerState.", createRasterizerStateResult);
#pragma endregion

			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.Width = device->m_window->GetClientWidth();
			viewport.Height = device->m_window->GetClientHeight();

			// sollte wsl nicht hier passieren
			// Todo: ev später refactoren
			device->m_deviceContext->RSSetState(device->m_rasterizerState.Get());
			device->m_deviceContext->RSSetViewports(1, &viewport);
			device->m_deviceContext->OMSetRenderTargets(1, &renderTargetView, device->m_depthStencilView.Get());
			device->m_deviceContext->OMSetDepthStencilState(device->m_depthStencilState.Get(), 1);

			return device;
		}

		std::variant<ID3D11Buffer*, dsr::dsr_error> Direct3dDevice::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData)
		{
			ID3D11Buffer* buffer = nullptr;
			HRESULT result = m_device->CreateBuffer(pDesc, pInitialData, &buffer);

			if (FAILED(result))
			{
				SafeRelease(buffer);
				return dsr_error("Error device was unable to create the Buffer.", result);
			}

			return buffer;
		}

		std::variant<ID3D11InputLayout*, dsr_error> Direct3dDevice::CreateInputLayout(
			const Direct3dShaderInputLayout& layout,
			ID3DBlob* shaderBlob) const
		{
			std::vector<Direct3dLayoutShaderInputLayoutDescription> descriptions = layout.GetLayout();
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;

			for (auto it = descriptions.begin(); it < descriptions.end(); ++it)
			{
				Direct3dLayoutShaderInputLayoutDescription description = *it;

				D3D11_INPUT_ELEMENT_DESC elementDesc;
				elementDesc.SemanticName = it->SemanticName.c_str();
				elementDesc.SemanticIndex = description.SemanticIndex;
				elementDesc.Format = description.Format;
				elementDesc.InputSlot = description.InputSlot;
				elementDesc.AlignedByteOffset = description.AlignedByteOffset;
				elementDesc.InputSlotClass = description.InputSlotClass;
				elementDesc.InstanceDataStepRate = description.InstanceDataStepRate;

				inputLayout.push_back(elementDesc);
			}

			ID3D11InputLayout* layoutPtr;
			HRESULT result = m_device->CreateInputLayout(inputLayout.data(), inputLayout.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &layoutPtr);

			if (FAILED(result))
			{
				SafeRelease(layoutPtr);
				return dsr_error("Error device was unable to create the Inputlayout.", result);
			}

			return layoutPtr;
		}

		void Direct3dDevice::SetInputLayout(ID3D11InputLayout* layout)
		{
			m_deviceContext->IASetInputLayout(layout);
		}

		void Direct3dDevice::Clear(const float& r, const float& g, const float& b, const float& a)
		{
			const float clearColor[4] = { r, g, b, a };
			m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		}

		void Direct3dDevice::SwapBuffers()
		{
			m_swapChain->Present(0, 0);
		}
	}
}
