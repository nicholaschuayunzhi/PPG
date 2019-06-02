struct PixelShaderInput
{
    float4 position: SV_POSITION;
    float2 TexCoord: TEXCOORD0;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
   return float4(1, 0, 1, 1);
}