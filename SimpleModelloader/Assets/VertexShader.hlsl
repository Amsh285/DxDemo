
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
}

struct AppData
{
    float3 position : POSITION;
    float2 texCoord : TXCOORD;
    float3 normal : NORMAL;
};

struct VertexShaderOutput
{
    float4 fragPosition : FRAGPOS;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput output;
 
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));

    output.fragPosition = mul(worldMatrix, float4(IN.position, 1.0f));
    output.normal = IN.normal;
    output.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    output.position = mul(mvp, float4(IN.position, 1.0f));

    return output;
}
