#pragma once

#include "DirectX/Buffers/Direct3dBuffer.h"
#include "DirectX/Direct3dDevice.h"
#include "ErrorHandling/DsrResult.h"

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

			Direct3dShader(const std::shared_ptr<Direct3dDevice>& device, const std::shared_ptr<TShader>& shader, const std::shared_ptr<ID3DBlob>& shaderBlob)
				: m_device(device), m_shader(shader), m_shaderBlob(shaderBlob)
			{
			}

			DsrResult SetConstantBuffer(const size_t& bRegister, DirectX::XMMATRIX& value);

		private:
			std::shared_ptr<Direct3dDevice> m_device;
			std::shared_ptr<TShader> m_shader;
			std::shared_ptr<ID3DBlob> m_shaderBlob;
			std::map<size_t, Direct3dBuffer> m_constantBuffers;
		};

		template<class TShader>
		inline DsrResult Direct3dShader<TShader>::SetConstantBuffer(const size_t& bRegister, DirectX::XMMATRIX& value)
		{
			auto it = std::find(m_constantBuffers.begin(), m_constantBuffers.end(), bRegister);

			if (it == m_constantBuffers.end())
			{
				D3D11_SUBRESOURCE_DATA subResourceData;
				ZeroMemory(&subResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
				subResourceData.pSysMem = &value;

				std::variant<Direct3dBuffer, dsr_error> createConstantBufferResult = Direct3dBuffer::CreateConstantBuffer(
					m_device,
					sizeof(DirectX::XMMATRIX),
					0, 0, D3D11_USAGE_DEFAULT,
					subResourceData);

				if (std::holds_alternative<dsr_error>(createConstantBufferResult))
				{
					const dsr_error& error = std::get<dsr_error>(createConstantBufferResult);
					return DsrResult(error.what(), error.GetResult());
				}

				m_constantBuffers[bRegister] = std::get<Direct3dBuffer>(createConstantBufferResult);
			}
			else
			{
				// use existing
			}

			return DsrResult::Success("Constant buffer setup successful.");
		}
	}
}
