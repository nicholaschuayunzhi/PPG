cbuffer PerObject : register(b0)
{
    matrix worldMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}

cbuffer PerApplication : register(b2)
{
    matrix projectionMatrix;
}

struct AppData
{
    float3 position: POSITION;
    float3 normal  : NORMAL;
    float2 texCoord: TEXCOORD;
};

struct VertexShaderOutput
{
    float4 position: SV_POSITION;
    float2 texCoord: TEXCOORD0;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput OUT;
    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    OUT.texCoord = IN.texCoord;
    return OUT;
}