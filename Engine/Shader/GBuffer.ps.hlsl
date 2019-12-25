#include "Common/Lighting.hlsli"
#include "Common/Material.hlsli"

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 pos      : POSITION0;
    float4 wPosition: POSITION1;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

struct GBufferOutput
{
    float4 diffuse : SV_TARGET0;
    float4 specular: SV_TARGET1;
    float4 normal  : SV_TARGET2;
};

static const float2 SpecPowerRange = { 0.0, 250.0 };

GBufferOutput main(PixelShaderInput IN)
{
    GBufferOutput OUT;
    float3 N = normalize(IN.normal);
    if (normalState != VERTEX_NORMALS)
    {
        N = CalculateNormal(IN.normal, IN.tangent, IN.binormal, IN.texCoord);
    }
    float specPow = (matShininess - SpecPowerRange.x) / SpecPowerRange.y;

    // PACK GBUFFER
    float4 diffuse = matDiffuse;
    if (useDiffuse)
        diffuse *= Diffuse.Sample(LinearSampler, IN.texCoord);
    float4 specular = float4(matSpecular.xyz, 0);
    if (useSpecular)
        specular *= Specular.Sample(LinearSampler, IN.texCoord).rrrr;

    OUT.diffuse = float4(diffuse.rgb, 0);
    OUT.specular.rgb = specular;
    OUT.specular.a = specPow;
    OUT.normal = float4(N * 0.5 + 0.5, 1);
    return OUT;
}
