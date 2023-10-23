#pragma once

#include "Direct3dShaderInputLayout.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dVertexBufferf
		{
		public:
			std::shared_ptr<ID3D11Buffer> GetVertexBuffer() const { return m_vertexBuffer; }
			std::shared_ptr<ID3D11Buffer> GetIndexBuffer() const { return m_indexBuffer; }
			Direct3dShaderInputLayout GetLayout() const { return m_shaderInputLayout; }

			Direct3dVertexBufferf(
				const std::shared_ptr<ID3D11Buffer>& vertexBuffer,
				const std::shared_ptr<ID3D11Buffer>& indexBuffer,
				const Direct3dShaderInputLayout& shaderInputLayout);
		private:
			std::shared_ptr<ID3D11Buffer> m_vertexBuffer;
			std::shared_ptr<ID3D11Buffer> m_indexBuffer;
			Direct3dShaderInputLayout m_shaderInputLayout;
		};
	}
}
