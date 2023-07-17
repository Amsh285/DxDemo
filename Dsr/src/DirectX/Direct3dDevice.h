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

			Direct3dDevice(const Direct3dDevice& other) = delete;
			Direct3dDevice& operator=(const Direct3dDevice& other) = delete;
		private:
			Direct3dDevice() {};

			ComPtr<ID3D11Device> m_device;
			ComPtr<ID3D11DeviceContext> m_deviceContext;
			ComPtr<IDXGISwapChain> m_swapChain;

			std::shared_ptr<dsr::windows::Window> m_window;
		};
	}
}
