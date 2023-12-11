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

SamplerState defaultSamplerState : register(s0);
Texture2D diffuseMap : register(t0);

struct PixelShaderInput
{
	matrix normalMatrix : MATNORM;

	float4 fragPosition : FRAGPOS;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 texCoord : TXCOORD;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
	float4 lightAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	float4 lightDiffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float4 lightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float4 lightPosition = CameraPosition;
	float4 lightPosition = float4(0.0f, 1.0f, -3.0f, 1.0f);

	float4 finalDiffuse = diffuseMap.Sample(defaultSamplerState, IN.texCoord);

	float4 ambientColor = Ka * lightAmbient;

	// diffuse
	float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(IN.normal, lightDirection.xyz), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * finalDiffuse);

	return ambientColor + diffuseColor;
}
