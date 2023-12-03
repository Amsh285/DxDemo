#pragma once

#include "DirectX/Textures/Direct3dShaderTexture2D.h"

namespace dsr
{
	namespace directX
	{
		class Material
		{
		public:
			float SpecularExponent;
			DirectX::XMFLOAT3 AmbientColor; //Ka
			DirectX::XMFLOAT3 DiffuseColor; //Kd
			DirectX::XMFLOAT3 EmissiveColor; //Ke
			float OpticalDensity; //Ni
			uint32_t IlluminationModel;

			Direct3dShaderTexture2D DiffuseMap;
		};
	}
}
