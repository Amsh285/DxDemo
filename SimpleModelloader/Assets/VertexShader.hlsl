
cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}
 
cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}
 
cbuffer PerObject : register(b2)
{
    matrix worldMatrix;
    matrix normalMatrix;
}

struct AppData
{
    float3 position : POSITION;
    float2 texCoord : TXCOORD;
    float3 normal : NORMAL;
};

struct VertexShaderOutput
{
    matrix normalMatrix : MATNORM;

    float4 fragPosition : FRAGPOS;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 texCoord : TXCOORD;
    float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput output;
 
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));

    output.normalMatrix = normalMatrix;
    output.fragPosition = mul(worldMatrix, float4(IN.position, 1.0f));
    output.normal = mul(normalMatrix, IN.normal);
    //output.normal = IN.normal;
    output.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    output.texCoord = IN.texCoord;
    output.position = mul(mvp, float4(IN.position, 1.0f));

    float3 edge1 = ddx(IN.position);
    float3 edge2 = ddy(IN.position);
    float2 deltaUV1 = ddx(IN.texCoord);
    float2 deltaUV2 = ddy(IN.texCoord);

    float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    float3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
    float3 biTangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

    float3 T = normalize(float3(mul(worldMatrix, float4(tangent, 0.0f))));
    float3 B = normalize(float3(mul(worldMatrix, float4(biTangent, 0.0f))));
    float3 N = normalize(float3(mul(worldMatrix, float4(IN.normal, 0.0f))));
    float3x3 TBN = float3x3(T, B, N);

    return output;
}




//shadows
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping