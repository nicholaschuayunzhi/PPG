struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 pos : POSITION0;
    float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

Texture2D src : register(t0);
sampler Sampler : register(s0);

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float2 texC = IN.texCoord;
    return src.Sample(Sampler, texC);
}
