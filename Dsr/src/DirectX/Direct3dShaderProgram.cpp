
#include "dsrpch.h"
#include "Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
		Direct3dShaderProgram::Direct3dShaderProgram(const Direct3dShader<ID3D11VertexShader>& vertexShader, const Direct3dShader<ID3D11PixelShader>& pixelShader)
			: VertexShader(vertexShader), PixelShader(pixelShader)
		{
			assert(vertexShader.GetShaderPtr());
			assert(vertexShader.GetShaderBlob());
			assert(pixelShader.GetShaderPtr());
			assert(pixelShader.GetShaderBlob());
		}
	}
}
