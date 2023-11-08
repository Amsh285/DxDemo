#pragma once

#include "DirectX/Buffers/Direct3dBuffer.h"

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

			void AddConstantBuffer(const std::string& id, const Direct3dBuffer& buffer);

			Direct3dShader(const std::shared_ptr<TShader>& shader, const std::shared_ptr<ID3DBlob>& shaderBlob)
				: m_shader(shader), m_shaderBlob(shaderBlob)
			{
			}
		private:
			std::shared_ptr<TShader> m_shader;
			std::shared_ptr<ID3DBlob> m_shaderBlob;
			std::vector<std::pair<std::string, Direct3dBuffer>> m_constantBuffers;
		};

		template<class TShader>
		inline void Direct3dShader<TShader>::AddConstantBuffer(const std::string& id, const Direct3dBuffer& buffer)
		{
			m_constantBuffers.push_back(std::make_pair<std::string, Direct3dBuffer>(id, buffer));
		}
	}
}
