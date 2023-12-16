#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Rendering/Data/VertexGroup.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"


namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			struct RenderData
			{
				// debug
				std::string Modelname;

				Direct3dVertexBufferf VertexBuffer;

				// https://github.com/microsoft/DirectXTK/wiki/Multistream-rendering-and-instancing
				data::Transform Transform;

				std::vector<std::shared_ptr<VertexGroup>> VertexGroups;

				RenderData(const Direct3dVertexBufferf& vertexBuffer)
					: VertexBuffer(vertexBuffer)
				{
				}
			};
			
			//this has to be refactored soon
			class Direct3dRenderUoW
			{
			public:
				Direct3dShaderProgram Shaders;
				std::vector<RenderData> RenderData;

				Direct3dRenderUoW(const Direct3dShaderProgram& shaderProgram);
			private:
			};
		}
	}
}
