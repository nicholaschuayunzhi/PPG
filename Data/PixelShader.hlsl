Texture2D Texture;
sampler Sampler;

struct PixelShaderInput
{
    float4 position: SV_POSITION;
    float2 texCoord: TEXCOORD0;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
   return Texture.Sample(Sampler, IN.texCoord);
}