
#include "dsrpch.h"
#include "Direct3dVertexBufferf.h"

namespace dsr
{
	namespace directX
	{
		Direct3dVertexBufferf::Direct3dVertexBufferf(
			const std::shared_ptr<ID3D11Buffer>& buffer,
			const Direct3dShaderInputLayout& shaderInputLayout)
			: m_buffer(buffer), m_shaderInputLayout(shaderInputLayout)
		{
		}
	}
}
