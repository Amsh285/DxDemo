#include "dsrpch.h"
#include "Direct3dDeviceBufferExtensions.h"


namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dVertexBufferf, dsr_error> LoadVertexBufferf(
			const std::shared_ptr<Direct3dDevice> device,
			const std::vector<float>& vertexData,
			const std::vector<uint32_t>& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout)
		{
			D3D11_BUFFER_DESC vertexBufferDescription;
			ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));

			vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDescription.ByteWidth = sizeof(float) * vertexData.size();
			vertexBufferDescription.CPUAccessFlags = 0;
			vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA subResourceData;
			ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			subResourceData.pSysMem = vertexData.data();

			std::variant<ID3D11Buffer*, dsr_error> createVertexBuffer = device->CreateBuffer(&vertexBufferDescription, &subResourceData);

			if (std::holds_alternative<dsr_error>(createVertexBuffer))
				return std::get<dsr_error>(createVertexBuffer);

			std::shared_ptr<ID3D11Buffer> vertexBufferPtr = std::shared_ptr<ID3D11Buffer>(
				std::get<ID3D11Buffer*>(createVertexBuffer),
				[](ID3D11Buffer* ptr) { SafeRelease(ptr); }
			);

			D3D11_BUFFER_DESC indexBufferDescription;
			ZeroMemory(&indexBufferDescription, sizeof(D3D11_BUFFER_DESC));

			indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDescription.ByteWidth = sizeof(uint32_t) * indexBuffer.size();
			indexBufferDescription.CPUAccessFlags = 0;
			indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
			subResourceData.pSysMem = indexBuffer.data();

			std::variant<ID3D11Buffer*, dsr_error> createIndexBuffer = device->CreateBuffer(&indexBufferDescription, &subResourceData);

			if (std::holds_alternative<dsr_error>(createIndexBuffer))
				return std::get<dsr_error>(createIndexBuffer);

			std::shared_ptr<ID3D11Buffer> indexBufferPtr = std::shared_ptr<ID3D11Buffer>(
				std::get<ID3D11Buffer*>(createIndexBuffer),
				[](ID3D11Buffer* ptr) { SafeRelease(ptr); }
			);

			return Direct3dVertexBufferf(vertexBufferPtr, indexBufferPtr, shaderInputLayout);
		}
	}
}
