#include "dsrpch.h"
#include "Direct3dBuffer.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dBuffer, dsr_error> Direct3dBuffer::CreateBuffer(const std::shared_ptr<Direct3dDevice> device, const D3D11_BUFFER_DESC& desc, const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData)
		{
			std::variant<ID3D11Buffer*, dsr_error> createBufferResult = device->CreateBuffer(&desc, &subResourceData.value_or(nullptr));

			if (std::holds_alternative<dsr_error>(createBufferResult))
				return std::get<dsr_error>(createBufferResult);

			std::shared_ptr<ID3D11Buffer> bufferPtr = std::shared_ptr<ID3D11Buffer>(
				std::get<ID3D11Buffer*>(createBufferResult),
				[](ID3D11Buffer* ptr) { SafeRelease(ptr); }
			);

			Direct3dBuffer buffer;
			buffer.m_description = desc;
			buffer.m_buffer = bufferPtr;

			return buffer;
		}
	}
}

