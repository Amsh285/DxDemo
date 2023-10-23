
#include "dsrpch.h"
#include "Direct3dVertexBufferf.h"

namespace dsr
{
	namespace directX
	{
		Direct3dVertexBufferf::Direct3dVertexBufferf(
			const std::shared_ptr<ID3D11Buffer>& vertexBuffer,
			const std::shared_ptr<ID3D11Buffer>& indexBuffer,
			const Direct3dShaderInputLayout& shaderInputLayout)
			: m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer), m_shaderInputLayout(shaderInputLayout)
		{
		}
	}
}
