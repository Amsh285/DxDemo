
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
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput OUT;
 
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    OUT.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
 
    return OUT;
}
