
//cbuffer MaterialBuffer : register(b0)
//{
//    float4 mat_color;
//}

cbuffer MaterialBuffer : register(b0)
{
    float mat_specularExponent;
    float3 mat_ambientColor;    // Ka
    float  pad0;
    float3 mat_diffuseColor;    // Kd
    float  pad1;
    float3 mat_emissiveColor;   // Ke
    float  pad2;
    float mat_opticalDensity;   // Ni
    uint  mat_illuminationModel;
    float  pad3;  // Padding to ensure 64 bytes
};


//cbuffer MaterialBuffer : register(b0)
//{
//    float mat_specularExponent;
//    float3 mat_ambientColor; //Ka
//    float3 mat_diffuseColor; //Kd
//    float3 mat_emissiveColor; //Ke
//    float mat_opticalDensity; //Ni
//    uint mat_illuminationModel;
//}
//

struct PixelShaderInput
{
    float4 fragPosition : FRAGPOS;
    float3 normal : NORMAL;
    float4 color : COLOR;
};
 
float4 main(PixelShaderInput IN) : SV_TARGET
{
    float4 ambientColor = float4(0.1f, 0.1f, 0.1f, 1.0f);

    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 lightPosition = float3(0.0f, 10.0f, -10.0f);

    float3 norm = normalize(IN.normal);
    float3 lightDirection = normalize(lightPosition - IN.fragPosition);
    float strength = max(dot(norm, lightDirection), 0.0);
    float4 diffuseColor = float4(strength * lightColor, 1.0f);

    // Use the MaterialBuffer to access material properties
    float4 materialAmbient = float4(mat_ambientColor, 1.0f);
    float4 materialDiffuse = float4(mat_diffuseColor, 1.0f);

    // Calculate the final color using material properties
    /*if (mat_illuminationModel == 1)
    {
        return IN.color * (materialDiffuse * (diffuseColor + ambientColor) * materialAmbient);
    }
    else
        return float4(0.8f, 0.8f, 0.8f, 1.0f);*/

    return IN.color * (materialDiffuse * (diffuseColor + ambientColor) * materialAmbient);
}
