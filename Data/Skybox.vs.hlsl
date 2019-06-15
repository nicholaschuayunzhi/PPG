cbuffer PerObject : register(b0)
{
    matrix model;
}

cbuffer PerFrame : register(b1)
{
    matrix view;
}

cbuffer PerApplication : register(b2)
{
    matrix projection;
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
    float3 texCoord: TEXCOORD;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput OUT;
    float4 worldPosition = mul(model, float4(IN.position, 1.0f));
    matrix vp = mul(projection, view);
    OUT.position = mul(vp, worldPosition);
    OUT.texCoord = mul(model, worldPosition.xyz);
    return OUT;
}
