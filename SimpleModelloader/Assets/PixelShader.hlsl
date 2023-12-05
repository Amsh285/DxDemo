
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

	uint UseDiffuseMap;
	uint UseBumpMap;
	uint UseAlphaMap;
	float pad1;
}

SamplerState defaultSamplerState : register(s0);
Texture2D diffuseMap : register(t0);

struct PixelShaderInput
{
	float4 fragPosition : FRAGPOS;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 texCoord : TXCOORD;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
	//maybe this shouldn´t be done test again when the shader has more functionality...
		//comments below can be switched to test the different results
		/*float4 materialAmbient = max(Ka, 0.0000000000000001f);
		float4 materialDiffuse = max(Kd, 0.0000000000000001f);
		float4 materialSpecular = max(Ks, 0.0000000000000001f);
		float4 materialEmissive = max(Ke, 0.0000000000000001f);*/

	float4 lightAmbient = float4(0.2f, 0.2, 0.2f, 1.0f);
	float4 lightDiffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float4 lightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 lightPosition = float4(0.0f, 10.0f, -10.0f, 1.0f);

	float4 ambientColor = Ka * lightAmbient;

	float4 norm = float4(normalize(IN.normal), 0.0f);
	float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(norm, lightDirection), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * Kd);

	if (UseDiffuseMap == 1)
	{
		float4 diffuseMapColor = diffuseMap.Sample(defaultSamplerState, IN.texCoord);
		return diffuseMapColor;
	}
	else
	{
		// Phong Shading
		float4 viewDir = float4(normalize(CameraPosition.xyz - IN.fragPosition.xyz), 0.0f);
		float4 reflectDir = float4(reflect(-lightPosition.xyz, norm.xyz), 0.0f);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), SpecularExponent);
		float4 specularColor = saturate(float4(lightSpecular.xyz * (spec * Ks.xyz), 0.0f));
		//float4 specularColor = saturate(float4(lightSpecular.xyz * (spec * materialSpecular.xyz), 0.0f));

		return ambientColor + diffuseColor + specularColor;
	}
}

