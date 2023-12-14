#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"
#include "DirectX/Shader/Data/PixelShaderData.h"
#include "DirectX/Textures/Direct3dShaderTexture2D.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			struct VertexGroup
			{
				//currently just for debugging
				std::string MaterialName;

				uint32_t StartIndexLocation;
				uint32_t IndexCount;

				PixelShaderData PSData;
				std::shared_ptr<Direct3dShader<ID3D11PixelShader>> PixelShader;
				std::vector<Direct3dShaderTexture2D> PSTextures2D;
			};

			struct RenderData
			{
				// debug
				std::string Modelname;

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
