
#include "dsrpch.h"
#include "Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
		Direct3dShaderProgram::Direct3dShaderProgram(
			const Direct3dShader<ID3D11VertexShader>& vertexShader,
			const Direct3dShader<ID3D11PixelShader>& pixelShader,
			const std::shared_ptr<ID3D11InputLayout>& vertexShaderInputLayout)
			: VertexShader(vertexShader), PixelShader(pixelShader), VertexShaderInputLayout(vertexShaderInputLayout)
		{
			assert(vertexShader.GetShaderPtr());
			assert(vertexShader.GetShaderBlob());
			assert(pixelShader.GetShaderPtr());
			assert(pixelShader.GetShaderBlob());
			assert(vertexShaderInputLayout);
		}
	}
}
