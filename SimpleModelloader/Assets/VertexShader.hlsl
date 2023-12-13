
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

    return output;
}




//shadows
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping