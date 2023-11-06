
#include "dsrpch.h"
#include "Direct3dVertexBufferf.h"

namespace dsr
{
	namespace directX
	{
		Direct3dVertexBufferf::Direct3dVertexBufferf(
			const Direct3dBuffer& vertexBuffer,
			const Direct3dBuffer& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout)
			: m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer), m_shaderInputLayout(shaderInputLayout)
		{
		}
	}
}
