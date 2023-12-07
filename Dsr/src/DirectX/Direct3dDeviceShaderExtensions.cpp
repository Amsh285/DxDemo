#include "dsrpch.h"
#include "Direct3dDeviceShaderExtensions.h"

namespace dsr
{
	namespace directX
	{
		std::variant<Direct3dShaderProgram, dsr_error> CreateShaderProgram(
			const std::shared_ptr<Direct3dDevice> device,
			const std::shared_ptr<Direct3dShader<ID3D11VertexShader>>& vertexShader,
			const std::shared_ptr<Direct3dShader<ID3D11PixelShader>>& pixelShader,
			const Direct3dShaderInputLayout& vertexShaderInputLayout)
		{
			assert(device);

			std::variant<ID3D11InputLayout*, dsr_error> createInputLayout = device->CreateInputLayout(vertexShaderInputLayout, vertexShader->GetShaderBlob().get());

			if (std::holds_alternative<dsr_error>(createInputLayout))
				return std::get<dsr::dsr_error>(createInputLayout);

			ID3D11InputLayout* inputLayoutPtr = std::get<ID3D11InputLayout*>(createInputLayout);
			assert(inputLayoutPtr);

			std::shared_ptr<ID3D11InputLayout> inputLayout = std::shared_ptr<ID3D11InputLayout>(inputLayoutPtr, [](ID3D11InputLayout* ptr) { SafeRelease(ptr); });
			Direct3dShaderProgram program = Direct3dShaderProgram(vertexShader, pixelShader, inputLayout);

			return program;
		}

		DsrResult SetConstantBuffer(const std::shared_ptr<Direct3dDevice> device, std::map<size_t, Direct3dBuffer>& target, const size_t& bRegister, const DirectX::XMMATRIX& value)
		{
			return SetConstantBuffer(device, target, bRegister, &value, sizeof(DirectX::XMMATRIX));
		}

		DsrResult SetConstantBuffer(const std::shared_ptr<Direct3dDevice> device, std::map<size_t, Direct3dBuffer>& target, const size_t& bRegister, const void* dataPtr, const size_t& dataSize)
		{
			auto it = target.find(bRegister);

			if (it == target.end())
			{
				D3D11_SUBRESOURCE_DATA subResourceData;
				ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
				subResourceData.pSysMem = dataPtr;

				std::variant<Direct3dBuffer, dsr_error> createConstantBufferResult = Direct3dBuffer::CreateConstantBuffer(
					device,
					dataSize,
					0, 0, D3D11_USAGE_DEFAULT,
					subResourceData);

				if (std::holds_alternative<dsr_error>(createConstantBufferResult))
				{
					const dsr_error& error = std::get<dsr_error>(createConstantBufferResult);
					return DsrResult(error.what(), error.GetResult());
				}

				target[bRegister] = std::get<Direct3dBuffer>(createConstantBufferResult);
			}
			else
			{
				std::shared_ptr<ID3D11Buffer> bufferPtr = it->second.GetBufferPtr();
				device->UpdateSubResource(bufferPtr.get(), 0, nullptr, dataPtr, 0, 0);
			}

			return DsrResult::Success("Constant buffer setup successful.");
		}
	}
}
