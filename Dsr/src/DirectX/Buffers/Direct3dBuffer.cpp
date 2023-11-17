#include "dsrpch.h"
#include "Direct3dBuffer.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dBuffer, dsr_error> Direct3dBuffer::CreateConstantBuffer(
			const std::shared_ptr<Direct3dDevice> device,
			const uint32_t& byteWidth,
			const uint32_t& cpuAccessFlags,
			const uint32_t& miscFlags,
			const D3D11_USAGE& usage,
			const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData)
		{
			D3D11_BUFFER_DESC desc;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = byteWidth;
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags;
			desc.StructureByteStride = 0;
			desc.Usage = usage;

			return CreateBuffer(device, desc, byteWidth, subResourceData);
		}

		std::variant<Direct3dBuffer, dsr_error> Direct3dBuffer::CreateIndexBuffer(const std::shared_ptr<Direct3dDevice> device, const std::vector<uint32_t>& indexData, const uint32_t& cpuAccessFlags, const uint32_t& miscFlags, const D3D11_USAGE& usage)
		{
			D3D11_BUFFER_DESC desc;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.ByteWidth = sizeof(uint32_t) * indexData.size();
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags;
			desc.StructureByteStride = 0;
			desc.Usage = usage;

			D3D11_SUBRESOURCE_DATA subResourceData;
			ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			subResourceData.pSysMem = indexData.data();

			return CreateBuffer(device, desc, indexData.size(), subResourceData);
		}

		std::variant<Direct3dBuffer, dsr_error> Direct3dBuffer::CreateVertexBufferf(
			const std::shared_ptr<Direct3dDevice> device,
			const std::vector<float>& vertexData,
			const uint32_t& cpuAccessFlags,
			const uint32_t& miscFlags,
			const D3D11_USAGE& usage
		)
		{
			D3D11_BUFFER_DESC desc;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = sizeof(float) * vertexData.size();
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags;
			desc.StructureByteStride = 0;
			desc.Usage = usage;

			D3D11_SUBRESOURCE_DATA subResourceData;
			ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			subResourceData.pSysMem = vertexData.data();

			return CreateBuffer(device, desc, vertexData.size(), subResourceData);
		}

		std::variant<Direct3dBuffer, dsr_error> Direct3dBuffer::CreateBuffer(
			const std::shared_ptr<Direct3dDevice> device,
			const D3D11_BUFFER_DESC& desc,
			const uint32_t& bufferSize,
			const std::optional<D3D11_SUBRESOURCE_DATA>& subResourceData)
		{
			const D3D11_SUBRESOURCE_DATA* subResourceDataPtr = nullptr;

			if (subResourceData.has_value())
				subResourceDataPtr = &subResourceData.value();

			std::variant<ID3D11Buffer*, dsr_error> createBufferResult = device->CreateBuffer(&desc, subResourceDataPtr);

			if (std::holds_alternative<dsr_error>(createBufferResult))
				return std::get<dsr_error>(createBufferResult);

			std::shared_ptr<ID3D11Buffer> bufferPtr = std::shared_ptr<ID3D11Buffer>(
				std::get<ID3D11Buffer*>(createBufferResult),
				[](ID3D11Buffer* ptr) { SafeRelease(ptr); }
			);

			return Direct3dBuffer(desc, bufferPtr, bufferSize);
		}

		Direct3dBuffer::Direct3dBuffer()
		{
			ZeroMemory(&m_description, sizeof(D3D11_BUFFER_DESC));
			m_bufferSize = 0;
		}

		Direct3dBuffer::Direct3dBuffer(const D3D11_BUFFER_DESC& desc, const std::shared_ptr<ID3D11Buffer>& bufferPtr, const uint32_t& bufferSize)
			: m_description(desc), m_buffer(bufferPtr), m_bufferSize(bufferSize)
		{
		}
	}
}

