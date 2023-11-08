#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			struct RenderData
			{
				Direct3dVertexBufferf VertexBuffer;

				// https://github.com/microsoft/DirectXTK/wiki/Multistream-rendering-and-instancing
				// later for instanced rendering, probably change to a Buffer
				data::Transform Transform;
			};
			
			class Direct3dRenderUoW
			{
			public:
				Direct3dShaderProgram GetProgram() const { return m_shaderProgram; }
				std::vector<RenderData> RenderData;

				Direct3dRenderUoW(const Direct3dShaderProgram& shaderProgram);
			private:
				Direct3dShaderProgram m_shaderProgram;
			};
		}
	}
}
