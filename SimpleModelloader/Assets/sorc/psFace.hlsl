cbuffer Material : register(b0)
{
	float4 Ka;
	float4 Kd;
	float4 Ks;
	float4 Ke;

	float SpecularExponent;
	float OpticalDensity;
	uint IlluminationModel;
	float pad0;

	float4 CameraPosition;
}

struct PixelShaderInput
{
	float4 fragPosition : FRAGPOS;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 texCoord : TXCOORD;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}