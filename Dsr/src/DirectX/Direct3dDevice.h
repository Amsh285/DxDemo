#pragma once

#include "ErrorHandling/createdirecd3ddevice_error.h"
#include "Windows/Window.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dDevice
		{
			template<class T>
			using ComPtr = Microsoft::WRL::ComPtr<T>;
			
			using DevicePtr = std::shared_ptr<Direct3dDevice>;
			using WindowPtr = std::shared_ptr<dsr::windows::Window>;
		public:
			static DevicePtr Create(const WindowPtr& window);

			void Clear();
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

			D3D_FEATURE_LEVEL m_featureLevel;
		};
	}
}
