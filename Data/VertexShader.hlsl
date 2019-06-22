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
    float3 tangent : TANGENT;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 pos      : POSITION0;
    float4 wPosition: POSITION1;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

VertexShaderOutput main(AppData IN)
{
    VertexShaderOutput OUT;
    OUT.wPosition = mul(model, float4(IN.position, 1.0f));
    matrix vp = mul(projection, view);
    OUT.position = mul(vp, OUT.wPosition);
    OUT.pos = OUT.position;
    OUT.texCoord = IN.texCoord;

    // assume a uniform scaling is observed
    // otherwise have have to multiply by transpose(inverse(model))
    // inverse should be calculated in the application (CPU)
    OUT.normal = normalize(mul(model, float4(IN.normal, 0)).xyz);
    OUT.tangent = normalize(mul(model, normalize(float4(IN.tangent, 0))).xyz);
    OUT.binormal = normalize(cross(OUT.normal, OUT.tangent));
    return OUT;
}
