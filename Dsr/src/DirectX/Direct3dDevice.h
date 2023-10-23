#pragma once

#include "ErrorHandling/dsr_error.h"
#include "Direct3dShaderInputLayout.h"
#include "Windows/Window.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dDevice
		{
			using DevicePtr = std::shared_ptr<Direct3dDevice>;
			using WindowPtr = std::shared_ptr<dsr::windows::Window>;
		public:
			static DevicePtr Create(const WindowPtr& window);

			std::variant<ID3D11Buffer*, dsr::dsr_error> CreateBuffer(
				const D3D11_BUFFER_DESC* pDesc,
				const D3D11_SUBRESOURCE_DATA* pInitialData);

			template<class ShaderType>
			std::variant<ShaderType*, dsr_error> CreateShader(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* classLinkage) const;

			template<>
			std::variant<ID3D11VertexShader*, dsr_error> CreateShader<ID3D11VertexShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) const
			{
				assert(pShaderBlob);

				ID3D11VertexShader* pVertexShader = nullptr;
				HRESULT result = m_device->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pVertexShader);

				if (FAILED(result))
				{
					SafeRelease(pVertexShader);
					return dsr_error("Error device was unable to create the Vertexshader.", result);
				}

				return pVertexShader;
			}

			template<>
			std::variant<ID3D11PixelShader*, dsr_error> CreateShader<ID3D11PixelShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) const
			{
				assert(pShaderBlob);

				ID3D11PixelShader* pPixelShader = nullptr;
				HRESULT result = m_device->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pPixelShader);

				if (FAILED(result))
				{
					SafeRelease(pPixelShader);
					return dsr_error("Error device was unable to create the Pixelshader.", result);
				}

				return pPixelShader;
			}

			std::variant<ID3D11InputLayout*, dsr_error> CreateInputLayout(
				const Direct3dShaderInputLayout& layout,
				ID3DBlob* shaderBlob) const;

			void SetInputLayout(ID3D11InputLayout* layout);

			template<class ShaderType>
			void UseShader(ShaderType* pShader, ID3D11ClassInstance* const* ppClassInstances, const uint32_t& NumClassInstances);

			template<>
			void UseShader<ID3D11VertexShader>(ID3D11VertexShader* pShader, ID3D11ClassInstance* const* ppClassInstances, const uint32_t& NumClassInstances)
			{
				assert(pShader);
				m_deviceContext->VSSetShader(pShader, ppClassInstances, NumClassInstances);
			}

			template<>
			void UseShader<ID3D11PixelShader>(ID3D11PixelShader* pShader, ID3D11ClassInstance* const* ppClassInstances, const uint32_t& NumClassInstances)
			{
				assert(pShader);
				m_deviceContext->PSSetShader(pShader, ppClassInstances, NumClassInstances);
			}

			void Clear(const float& r, const float& g, const float& b, const float& a);
			void SwapBuffers();

			Direct3dDevice(const Direct3dDevice& other) = delete;
			Direct3dDevice& operator=(const Direct3dDevice& other) = delete;
		private:
			Direct3dDevice() {};

			ComPtr<ID3D11Device> m_device;
			ComPtr<ID3D11DeviceContext> m_deviceContext;
			ComPtr<IDXGISwapChain> m_swapChain;
			ComPtr<ID3D11RenderTargetView> m_renderTargetView;
			ComPtr<ID3D11DepthStencilView> m_depthStencilView;
			ComPtr<ID3D11DepthStencilState> m_depthStencilState;
			ComPtr<ID3D11RasterizerState> m_rasterizerState;

			std::shared_ptr<dsr::windows::Window> m_window;

			std::vector<D3D_FEATURE_LEVEL> m_requestedFeatureLevels = {
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

			std::unique_ptr<D3D_FEATURE_LEVEL> m_featureLevel;
		};
	}
}
