TextureCube SkyboxTexture : register(t0);
sampler Sampler: register(s0);

struct PixelShaderInput
{
    float4 position: SV_POSITION;
    float3 texCoord: TEXCOORD;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
    return SkyboxTexture.Sample(Sampler, IN.texCoord);
}
