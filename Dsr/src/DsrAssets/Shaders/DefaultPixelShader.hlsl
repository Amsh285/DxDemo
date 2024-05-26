
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
}

cbuffer Camera : register(b1)
{
    float4 CameraPosition;
}

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
	float4 lightAmbient = float4(0.2f, 0.2, 0.2f, 1.0f);
	float4 lightDiffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);
	float4 lightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// Todo: remove that and add dynamic lightsources.
	float4 pointLightPosition = float4(0.0f, 10.0f, -10.0f, 1.0f);
	
    float4 directionalLight = float4(-0.2f, -1.0f, -0.3f, 0.0f);

	float4 ambientColor = Ka * lightAmbient;

	float4 norm = float4(normalize(IN.normal), 0.0f);
    //float4 lightDirection = float4(normalize(pointLightPosition.xyz - IN.fragPosition.xyz), 0.0f);
    float4 lightDirection = normalize(-directionalLight);
	float diffuse_strength = max(dot(norm, lightDirection), 0.0);
	float4 diffuseColor = lightDiffuse * (diffuse_strength * Kd);

	float4 viewDir = float4(normalize(CameraPosition.xyz - IN.fragPosition.xyz), 0.0f);
    //float4 reflectDir = float4(reflect(-pointLightPosition.xyz, norm.xyz), 0.0f);
    float4 reflectDir = float4(reflect(-lightDirection.xyz, norm.xyz), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), SpecularExponent);
	float4 specularColor = saturate(float4(lightSpecular.xyz * (spec * Ks.xyz), 0.0f));

	return ambientColor + diffuseColor + specularColor;
}

