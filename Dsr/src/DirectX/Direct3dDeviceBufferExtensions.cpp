#include "dsrpch.h"
#include "Direct3dDeviceBufferExtensions.h"


namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dVertexBufferf, dsr_error> SetupVertexBufferf(
			const std::shared_ptr<Direct3dDevice> device,
			const std::vector<float>& vertexData,
			const std::vector<uint32_t>& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout)
		{
			std::variant<Direct3dBuffer, dsr_error> createVertexBuffer = Direct3dBuffer::CreateVertexBufferf(device, vertexData);
			if (std::holds_alternative<dsr_error>(createVertexBuffer))
				return std::get<dsr_error>(createVertexBuffer);

			std::variant<Direct3dBuffer, dsr_error> createIndexBuffer = Direct3dBuffer::CreateIndexBuffer(device, indexBuffer);
			if (std::holds_alternative<dsr_error>(createIndexBuffer))
				return std::get<dsr_error>(createIndexBuffer);

			return Direct3dVertexBufferf(
				std::get<Direct3dBuffer>(createVertexBuffer),
				std::get<Direct3dBuffer>(createIndexBuffer),
				shaderInputLayout
			);
		}
	}
}
