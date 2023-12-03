#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"
#include "DirectX/Shader/Data/Material.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			struct VertexGroup
			{
				uint32_t StartIndexLocation;
				uint32_t IndexCount;
				Material Material;
			};

			struct RenderData
			{
				Direct3dVertexBufferf VertexBuffer;

				// https://github.com/microsoft/DirectXTK/wiki/Multistream-rendering-and-instancing
				// later for instanced rendering, probably change to a Buffer
				data::Transform Transform;

				std::vector<VertexGroup> VertexGroups;

				RenderData(const Direct3dVertexBufferf& vertexBuffer)
					: VertexBuffer(vertexBuffer)
				{
				}
			};
			
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
