#pragma once

#include "DirectX/Shader/Data/PixelShaderData.h"
#include "DirectX/Shader/Direct3dShader.h"
#include "DirectX/Textures/Direct3dShaderTexture2D.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			class VertexGroup
			{
			public:
				std::string MaterialName;

				uint32_t StartIndexLocation;
				uint32_t IndexCount;
				uint32_t SortOrder;

				PixelShaderData PSData;
				std::shared_ptr<Direct3dShader<ID3D11PixelShader>> PixelShader;
				std::vector<Direct3dShaderTexture2D> PSTextures2D;
			private:
			};
		}
	}
}
