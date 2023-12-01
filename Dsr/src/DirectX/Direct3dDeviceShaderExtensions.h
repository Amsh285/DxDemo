#pragma once

#include "Direct3dDevice.h"
#include "Shader/Direct3dShaderInputLayout.h"
#include "Shader/Direct3dShader.h"
#include "Shader/Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
#ifdef _DEBUG
#define SHADERCOMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG
#else
#define SHADERCOMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS
#endif // DEBUG

		template<class TShader>
		std::variant<std::shared_ptr<Direct3dShader<TShader>>, dsr_error> LoadShaderFromFile(
			const std::shared_ptr<Direct3dDevice> device,
			const std::wstring& fileName,
			const std::string& profile,
			const std::string& entryPoint = "main"
		)
		{
			ID3DBlob* pShaderBlob = nullptr;
			ID3DBlob* pErrorBlob = nullptr;

			HRESULT hr = D3DCompileFromFile(
				fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entryPoint.c_str(), profile.c_str(), SHADERCOMPILE_FLAGS, 0,
				&pShaderBlob, &pErrorBlob
			);

			if (FAILED(hr))
			{
				std::string errorMessage;

				if (pErrorBlob)
				{
					errorMessage = static_cast<char*>(pErrorBlob->GetBufferPointer());
					OutputDebugStringA(errorMessage.c_str());
				}

				SafeRelease(pShaderBlob);
				SafeRelease(pErrorBlob);

				std::string msg = "error compiling shader: " + std::string(fileName.begin(), fileName.end()) + ". " + errorMessage;
				return dsr_error(msg, hr);
			}

			SafeRelease(pErrorBlob);

			std::variant<TShader*, dsr_error> createShaderResult = device->CreateShader<TShader>(pShaderBlob, nullptr);

			if (std::holds_alternative<dsr_error>(createShaderResult))
			{
				SafeRelease(pShaderBlob);
				dsr_error createShaderError = std::get<dsr_error>(createShaderResult);

				return dsr_error::Attach(
					"error creating shader: " + std::string(fileName.begin(), fileName.end()) + ". ",
					createShaderError
				);
			}

			TShader* pShader = std::get<TShader*>(createShaderResult);
			assert(pShader);
			assert(pShaderBlob);

			std::shared_ptr<TShader> shaderPtr = std::shared_ptr<TShader>(pShader, [](TShader* ptr) { SafeRelease(ptr); });
			std::shared_ptr<ID3DBlob> shaderBlobPtr = std::shared_ptr<ID3DBlob>(pShaderBlob, [](ID3DBlob* ptr) { SafeRelease(ptr); });
			std::shared_ptr<Direct3dShader<TShader>> shader = std::make_shared<Direct3dShader<TShader>>(shaderPtr, shaderBlobPtr);

			return shader;
		}

		std::variant<Direct3dShaderProgram, dsr_error> CreateShaderProgram(
			const std::shared_ptr<Direct3dDevice> device,
			const std::shared_ptr<Direct3dShader<ID3D11VertexShader>>& vertexShader,
			const std::shared_ptr<Direct3dShader<ID3D11PixelShader>>& pixelShader,
			const Direct3dShaderInputLayout& vertexShaderInputLayout
		);

		template<class TShader>
		DsrResult SetConstantBuffer(
			const std::shared_ptr<Direct3dDevice> device,
			std::shared_ptr<Direct3dShader<TShader>> target,
			const size_t& bRegister,
			const DirectX::XMMATRIX& value)
		{
			return SetConstantBuffer(device, target, bRegister, &value, sizeof(DirectX::XMMATRIX));
		}

		template<class TShader>
		DsrResult SetConstantBuffer(
			const std::shared_ptr<Direct3dDevice> device,
			std::shared_ptr<Direct3dShader<TShader>> target,
			const size_t& bRegister,
			const void* dataPtr,
			const size_t& dataSize)
		{
			auto it = target->ConstantBuffers.find(bRegister);

			if (it == target->ConstantBuffers.end())
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

				target->ConstantBuffers[bRegister] = std::get<Direct3dBuffer>(createConstantBufferResult);
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
