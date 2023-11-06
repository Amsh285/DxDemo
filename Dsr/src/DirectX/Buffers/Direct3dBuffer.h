#pragma once

#include "DirectX/Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dBuffer
		{
		public:
			static std::variant<Direct3dBuffer, dsr_error> CreateBuffer(
				const std::shared_ptr<Direct3dDevice> device,
				const D3D11_BUFFER_DESC& desc,
				const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData);

			D3D11_BUFFER_DESC GetBufferDescription() const { return m_description; }
			std::shared_ptr<ID3D11Buffer> GetBufferPtr() const { return m_buffer; }

		private:
			Direct3dBuffer() {};

			D3D11_BUFFER_DESC m_description;
			std::shared_ptr<ID3D11Buffer> m_buffer;
		};
	}
}
