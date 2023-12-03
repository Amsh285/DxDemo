#pragma once

#include "DirectX/Textures/Direct3dShaderTexture2D.h"

namespace dsr
{
	namespace directX
	{
		struct Material
		{
			float SpecularExponent;
			DirectX::XMFLOAT3 AmbientColor; //Ka
			DirectX::XMFLOAT3 DiffuseColor; //Kd
			DirectX::XMFLOAT3 EmissiveColor; //Ke
			float OpticalDensity; //Ni
			uint32_t IlluminationModel;

			std::optional<Direct3dShaderTexture2D> DiffuseMap;
		};
	}
}
