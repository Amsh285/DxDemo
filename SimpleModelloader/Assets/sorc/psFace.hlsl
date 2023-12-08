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
Texture2D normalMap : register(t1);

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
	// does not work. must probably implement a BSDF (Bidirectional Scattering Distribution Function)

	float4 lightAmbient = float4(0.2f, 0.2, 0.2f, 1.0f);
	float4 lightDiffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float4 lightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	/*float4 lightPosition = float4(0.0f, 0.0f, -5.0f, 1.0f);*/
	float4 lightPosition = float4(0.0f, 10.0f, -10.0f, 1.0f);

	
	float4 ambientColor = Ka * lightAmbient;
	float3 normalMapSample = normalMap.Sample(defaultSamplerState, IN.texCoord).xyz;
	normalMapSample = normalize(normalMapSample * 2.0 - 1.0);
	normalMapSample.z = -normalMapSample.z;
	/*
	normalMapSample.z = -normalMapSample.z;*/
	//float3 worldNormalSample = mul(float4(normalMapSample, 0.0), IN.normalMatrix).xyz;
	float3 perturbedNormal = normalize(lerp(IN.normal, normalMapSample, 0.4f));
	/*float3 perturbedNormal = normalize(IN.normal + normalMapSample * 0.1f);*/

	float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(perturbedNormal, lightDirection.xyz), 0.0);

	float4 diffuseMapColor = diffuseMap.Sample(defaultSamplerState, IN.texCoord);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * diffuseMapColor);
	return ambientColor + diffuseColor;

	/*float4 ambientColor = Ka * lightAmbient;
	float4 diffuseMapColor = diffuseMap.Sample(defaultSamplerState, IN.texCoord);

	float4 norm = float4(normalize(IN.normal), 0.0f);
	float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(norm, lightDirection), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * diffuseMapColor);

	float4 viewDir = float4(normalize(CameraPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float4 reflectDir = float4(reflect(-lightPosition.xyz, norm.xyz), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SpecularExponent);
	float4 specularColor = saturate(float4(lightSpecular.xyz * (spec * Ks.xyz), 0.0f));
	
	return ambientColor + diffuseColor + specularColor;*/
}
