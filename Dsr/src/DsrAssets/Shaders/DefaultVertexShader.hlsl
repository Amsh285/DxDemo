
cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}

cbuffer RenderTransform : register(b2)
{
    matrix worldMatrix;
    matrix normalMatrix;
}

struct VertexBufferData
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

VertexShaderOutput main(VertexBufferData IN)
{
    VertexShaderOutput output;

    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));

    output.normalMatrix = normalMatrix;
    output.fragPosition = mul(worldMatrix, float4(IN.position, 1.0f));
    //output.normal = mul(normalMatrix, IN.normal);
    output.normal = mul(normalMatrix, float4(IN.normal.xyz, 0.0f));

    output.color = float4(0.6f, 0.6f, 0.6f, 1.0f);
    output.texCoord = IN.texCoord;
    output.position = mul(mvp, float4(IN.position, 1.0f));

    return output;
}
