#pragma once

#include "Direct3dShader.h"
#include "Direct3dShaderInputLayout.h"

namespace dsr
{
	namespace directX
	{
		class Direct3dShaderProgram
		{
		public:
			Direct3dShader<ID3D11VertexShader> VertexShader;
			Direct3dShader<ID3D11PixelShader> PixelShader;
			std::shared_ptr<ID3D11InputLayout> VertexShaderInputLayout;

			std::optional<Direct3dShader<ID3D11HullShader>> HullShader;
			std::optional<Direct3dShader<ID3D11DomainShader>> DomainShader;
			std::optional<Direct3dShader<ID3D11GeometryShader>> GeometryShader;

			Direct3dShaderProgram(
				const Direct3dShader<ID3D11VertexShader>& vertexShader,
				const Direct3dShader<ID3D11PixelShader>& pixelShader,
				const std::shared_ptr<ID3D11InputLayout>& vertexShaderInputLayout);
		private:
		};
	}
}
