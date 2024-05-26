#pragma once

namespace dsr
{
	namespace directX
	{
        struct PixelShaderData
        {
            DirectX::XMFLOAT4 AmbientColor;
            DirectX::XMFLOAT4 DiffuseColor;
            DirectX::XMFLOAT4 EmissiveColor;
            DirectX::XMFLOAT4 SpecularColor;

            float SpecularExponent;
            float OpticalDensity;
            uint32_t IlluminationModel;
            float pad0;

            PixelShaderData()
                : SpecularExponent(0.0f),  // set your default values here
                AmbientColor(0.0f, 0.0f, 0.0f, 1.0f),
                DiffuseColor(0.0f, 0.0f, 0.0f, 1.0f),
                EmissiveColor(0.0f, 0.0f, 0.0f, 1.0f),
                SpecularColor(0.0f, 0.0f, 0.0f, 1.0f),
                OpticalDensity(1.0f),
                IlluminationModel(0),
                pad0(0.0f)
            {
            }
        };
	}
}
