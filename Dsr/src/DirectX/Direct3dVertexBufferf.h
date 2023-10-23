#pragma once

#include "Direct3dShaderInputLayout.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dVertexBufferf
		{
		public:
			std::shared_ptr<ID3D11Buffer> GetBuffer() const { return m_buffer; }
			Direct3dShaderInputLayout GetLayout() const { return m_shaderInputLayout; }

			Direct3dVertexBufferf(
				const std::shared_ptr<ID3D11Buffer>& buffer,
				const Direct3dShaderInputLayout& shaderInputLayout);
		private:
			std::shared_ptr<ID3D11Buffer> m_buffer;
			Direct3dShaderInputLayout m_shaderInputLayout;
		};
	}
}

