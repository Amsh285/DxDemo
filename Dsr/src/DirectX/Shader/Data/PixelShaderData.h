#pragma once

namespace dsr
{
	namespace directX
	{
        struct PixelShaderData
        {
            float SpecularExponent;
            DirectX::XMFLOAT3 AmbientColor;
            float pad0;
            DirectX::XMFLOAT3 DiffuseColor;
            float pad1;
            DirectX::XMFLOAT3 EmissiveColor;
            float pad2;
            float OpticalDensity;
            uint32_t IlluminationModel;
            float pad3;
            DirectX::XMFLOAT3 CameraPosition;
            float pad4;
        };
	}
}
