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
	}
}
