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

		private:
			Direct3dShader() {};
			std::shared_ptr<TShader> m_shader;
		};

#ifdef _DEBUG
		static const UINT s_shaderCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
		static const UINT s_shaderCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif // DEBUG

		template<class TShader>
		Direct3dShader<TShader> LoadShaderFromFile(
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
				entryPoint.c_str(), profile.c_str(), s_shaderCompileFlags, 0,
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
				throw dsr_error(msg, hr);
			}

			TShader* pShader;

			try
			{
				SafeRelease(pShaderBlob);
				SafeRelease(pErrorBlob);

				pShader = device->CreateShader<TShader>(pShaderBlob, nullptr);
				assert(pShader);

				Direct3dShader<TShader> shader;
				shader.m_shader = std::shared_ptr<TShader>(pShader, [](TShader* ptr) { SafeRelease(ptr); });
				return shader;
			}
			catch (const std::exception& e)
			{
				SafeRelease(pShader);
				std::string msg = "error creating shader: " + std::string(fileName.begin(), fileName.end()) + ". " + e.what();
				throw dsr::dsr_error(msg, E_FAIL);
			}
		}
	}
}
