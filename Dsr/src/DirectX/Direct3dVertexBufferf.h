#pragma once

#include "Direct3dShaderInputLayout.h"
#include "Buffers/Direct3dBuffer.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dVertexBufferf
		{
		public:
			Direct3dBuffer GetVertexBuffer() const { return m_vertexBuffer; }
			Direct3dBuffer GetIndexBuffer() const { return m_indexBuffer; }
			Direct3dShaderInputLayout GetLayout() const { return m_shaderInputLayout; }

			Direct3dVertexBufferf(
				const Direct3dBuffer& vertexBuffer,
				const Direct3dBuffer& indexBuffer,
				const Direct3dShaderInputLayout& shaderInputLayout);
		private:
			Direct3dBuffer m_vertexBuffer;
			Direct3dBuffer m_indexBuffer;
			Direct3dShaderInputLayout m_shaderInputLayout;
		};
	}
}
