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

            DirectX::XMFLOAT4 CameraPosition;

            uint32_t UseDiffuseMap;
            uint32_t UseBumpMap;
            uint32_t UseAlphaMap;
            float pad1;

            PixelShaderData()
                : SpecularExponent(0.0f),  // set your default values here
                AmbientColor(0.0f, 0.0f, 0.0f, 1.0f),
                DiffuseColor(0.0f, 0.0f, 0.0f, 1.0f),
                EmissiveColor(0.0f, 0.0f, 0.0f, 1.0f),
                SpecularColor(0.0f, 0.0f, 0.0f, 1.0f),
                OpticalDensity(1.0f),
                IlluminationModel(0),
                CameraPosition(0.0f, 0.0f, 0.0f, 1.0f),
                pad0(0.0f), pad1(0.0f)
            {
                UseDiffuseMap = UseBumpMap = UseAlphaMap = 0;
            }
        };
	}
}
