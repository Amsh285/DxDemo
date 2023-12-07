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

			// this should be fixed. copy functions are deleted to prevent copy of constantbuffers, leading to errors due to out of date buffers.
			// But this makes it hat work with. Something should be done with the ConstantBuffers Container.
			// Maybe factory and let shaders only exist as shared ptrs.
			std::map<size_t, Direct3dBuffer> ConstantBuffers;

			Direct3dShader(const std::shared_ptr<TShader>& shader, const std::shared_ptr<ID3DBlob>& shaderBlob)
				: m_shader(shader), m_shaderBlob(shaderBlob)
			{
			}

			Direct3dShader(const Direct3dShader& other) = delete;
			Direct3dShader& operator=(const Direct3dShader& other) = delete;
		private:
			std::shared_ptr<TShader> m_shader;
			std::shared_ptr<ID3DBlob> m_shaderBlob;
		};
	}
}
