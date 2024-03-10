
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

struct VertexufferData
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VertexShaderOutput main(VertexufferData IN)
{
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    
    VertexShaderOutput output;
    output.Position = mul(mvp, float4(IN.Position, 1.0f));
    output.Color = IN.Color;
    return output;
}
