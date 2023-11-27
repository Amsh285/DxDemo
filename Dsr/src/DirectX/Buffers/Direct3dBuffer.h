#pragma once

#include "DirectX/Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dBuffer
		{
		public:
			static std::variant<Direct3dBuffer, dsr_error> CreateConstantBuffer(
				const std::shared_ptr<Direct3dDevice> device,
				const uint32_t& byteWidth,
				const uint32_t& cpuAccessFlags = 0,
				const uint32_t& miscFlags = 0,
				const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT,
				const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData = std::nullopt
			);

			static std::variant<Direct3dBuffer, dsr_error> CreateIndexBuffer(
				const std::shared_ptr<Direct3dDevice> device,
				const std::vector<uint32_t>& indexData,
				const uint32_t& cpuAccessFlags = 0,
				const uint32_t& miscFlags = 0,
				const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT
			);

			static std::variant<Direct3dBuffer, dsr_error> CreateVertexBufferf(
				const std::shared_ptr<Direct3dDevice> device,
				const std::vector<float>& vertexData,
				const uint32_t& cpuAccessFlags = 0,
				const uint32_t& miscFlags = 0,
				const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT
			);

			static std::variant<Direct3dBuffer, dsr_error> CreateBuffer(
				const std::shared_ptr<Direct3dDevice> device,
				const D3D11_BUFFER_DESC& desc,
				const uint32_t& bufferSize,
				const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData);

			D3D11_BUFFER_DESC GetBufferDescription() const { return m_description; }
			std::shared_ptr<ID3D11Buffer> GetBufferPtr() const { return m_buffer; }
			uint32_t GetBufferSize() const { return m_bufferSize; }

			Direct3dBuffer();
			Direct3dBuffer(const D3D11_BUFFER_DESC& desc, const std::shared_ptr<ID3D11Buffer>& bufferPtr, const uint32_t& bufferSize);
		private:
			D3D11_BUFFER_DESC m_description;
			std::shared_ptr<ID3D11Buffer> m_buffer;
			uint32_t m_bufferSize;
		};
	}
}
