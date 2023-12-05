
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

    output.fragPosition = mul(worldMatrix, float4(IN.position, 1.0f));
    output.normal = mul(normalMatrix, IN.normal);
    //output.normal = IN.normal;
    output.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    output.texCoord = IN.texCoord;
    output.position = mul(mvp, float4(IN.position, 1.0f));

    return output;
}

// Todo:
//Whenever we apply a non - uniform scale(note: a uniform scale only changes the normal's magnitude, not its direction, which is easily fixed by normalizing it) the normal vectors are not perpendicular to the corresponding surface anymore which distorts the lighting.
//
//    The trick of fixing this behavior is to use a different model matrix specifically tailored for normal vectors.This matrix is called the normal matrix and uses a few linear algebraic operations to remove the effect of wrongly scaling the normal vectors.If you want to know how this matrix is calculated I suggest the following article.
//
//    The normal matrix is defined as 'the transpose of the inverse of the upper-left 3x3 part of the model matrix'.Phew, that's a mouthful and if you don't really understand what that means, don't worry; we haven't discussed inverse and transpose matrices yet.Note that most resources define the normal matrix as derived from the model - view matrix, but since we're working in world space (and not in view space) we will derive it from the model matrix.
//
//    In the vertex shader we can generate the normal matrix by using the inverse and transpose functions in the vertex shader that work on any matrix type.Note that we cast the matrix to a 3x3 matrix to ensure it loses its translation properties and that it can multiply with the vec3 normal vector :


//shadows
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping