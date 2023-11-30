
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
    float3 lightPosition = float3(0.0f, 10.0f, -200.0f);

    float3 norm = normalize(IN.normal);
    float3 lightDirection = normalize(lightPosition - IN.fragPosition);
    float strength = max(dot(norm, lightDirection), 0.0);
    float4 diffuseColor = float4(strength * lightColor, 1.0f);

    return IN.color * (ambientColor + diffuseColor);
}
