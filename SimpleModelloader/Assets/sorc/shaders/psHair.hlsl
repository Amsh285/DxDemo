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
	float4 lightAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	float4 lightDiffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float4 lightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float4 lightPosition = CameraPosition;
	float4 lightPosition = float4(0.0f, 1.0f, -3.0f, 1.0f);

	float4 ambientColor = Ka * lightAmbient;

	float4 diffuseMapColor = diffuseMap.Sample(defaultSamplerState, IN.texCoord);
	float minValue = 0.0f;
	float maxValue = 0.455f;
	float4 mappedValue = saturate((diffuseMapColor - minValue) / (maxValue - minValue));

	// configure color areas
	float4 finalDiffuse = lerp(float4(0.000000, 0.000000, 0.000000, 1.000000), float4(0.040874, 0.070489, 0.166997, 1.000000), mappedValue);

	// pertub normalvector
	float3 normalMapSample = normalMap.Sample(defaultSamplerState, IN.texCoord).xyz;
	normalMapSample = normalize(normalMapSample * 2.0 - 1.0);
	normalMapSample.z = -normalMapSample.z;
	float3 perturbedNormal = normalize(IN.normal + normalMapSample * 1.0f);

	// diffuse
	float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(perturbedNormal, lightDirection.xyz), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * finalDiffuse);

	// wsl eher metallic
	// specular
	float4 viewDir = float4(normalize(CameraPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float3 reflectDir = reflect(-lightPosition.xyz, perturbedNormal);
	float4 specularMapColor = Ks;

	// Calculate the Fresnel term using Schlick's approximation
	float cosTheta = dot(viewDir.xyz, reflectDir);
	float fresnel = pow(1.0 - cosTheta, 2); // Adjust the power value as needed

	// Adjust the strength of the specular reflection
	float specularStrength = 0.1f;

	// Blend between reflection and refraction based on Fresnel term
	float3 specularColor = lerp(
		lightSpecular.xyz * specularMapColor.xyz,
		diffuseMapColor.xyz,
		fresnel
	);

	return ambientColor + diffuseColor + float4(specularColor, 1.0f) * specularStrength;

	// diffuse
	/*float4 lightDirection = float4(normalize(lightPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float diffuse_strength = max(dot(perturbedNormal, lightDirection.xyz), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * finalDiffuse);

	float4 viewDir = float4(normalize(CameraPosition.xyz - IN.fragPosition.xyz), 0.0f);
	float4 reflectDir = float4(reflect(-lightPosition.xyz, perturbedNormal.xyz), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SpecularExponent);
	float4 specularColor = saturate(float4(lightSpecular.xyz * (spec * Ks.xyz), 0.0f));

	return ambientColor + diffuseColor + specularColor;*/
}
