#pragma once

#include "DirectX/Buffers/Direct3dBuffer.h"
#include "DirectX/Direct3dDevice.h"
#include "ErrorHandling/NotFoundError.h"

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



		private:
			std::shared_ptr<Direct3dDevice> m_device;
			std::shared_ptr<TShader> m_shader;
			std::shared_ptr<ID3DBlob> m_shaderBlob;
			std::map<size_t, Direct3dBuffer> m_constantBuffers;
		};
	}
}
