#pragma once

#include "Direct3dDevice.h"

namespace dsr
{
	namespace directX
	{
		template<class TShader>
		class Direct3dShader
		{
		public:
			std::shared_ptr<TShader> GetShaderPtr() const { return m_shader; }
			std::shared_ptr<ID3DBlob> GetShaderBlob() const { return m_shaderBlob; }

			Direct3dShader(const std::shared_ptr<TShader>& shader, const std::shared_ptr<ID3DBlob>& shaderBlob)
				: m_shader(shader), m_shaderBlob(shaderBlob)
			{
			}

		private:
			std::shared_ptr<TShader> m_shader;
			std::shared_ptr<ID3DBlob> m_shaderBlob;
		};

#ifdef _DEBUG
#define SHADERCOMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG
#else
#define SHADERCOMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS
#endif // DEBUG

		template<class TShader>
		std::variant<Direct3dShader<TShader>, dsr_error> LoadShaderFromFile(
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
			Direct3dShader<TShader> shader(shaderPtr, shaderBlobPtr);
			return shader;
		}
	}
}
