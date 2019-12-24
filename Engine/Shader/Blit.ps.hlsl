struct PixelShaderInput // see Fullscreen.vs.hlsl
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D src : register(t0);
sampler Sampler : register(s0);

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float2 texC = IN.texCoord;
    return src.Sample(Sampler, texC);
}
