#include "Common/Lighting.hlsli"

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

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);

    float3 N = normalize(IN.normal);
    if (normalState != VERTEX_NORMALS)
    {
        N = CalculateNormal(IN.normal, IN.tangent, IN.binormal, IN.texCoord);
    }

    ShadingInfo shadingInfo;
    shadingInfo.posW = IN.wPosition;
    shadingInfo.normal = N;
    shadingInfo.viewDir = normalize(eyePosition.xyz - IN.wPosition.xyz);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        switch (abs(light.lightType))
        {
            case DIRECTIONAL_LIGHT:
                CalculateDirectionalLight(light, shadingInfo, diffuse, specular);
                break;
            case POINT_LIGHT:
                CalculatePointLight(light, shadingInfo, diffuse, specular);
                break;
            case SPOT_LIGHT:
                CalculateSpotLight(light, shadingInfo, diffuse, specular);
                break;
            default:
                return float4(1, 0, 1, 1);
        }
    }

    float4 finalDiffuse = matDiffuse;
    if (useDiffuse)
        finalDiffuse *= Diffuse.Sample(Sampler, IN.texCoord);
    float4 finalSpecular = matSpecular;
    if (useSpecular)
        finalSpecular *= Specular.Sample(Sampler, IN.texCoord).rrrr;
    float4 final =
        matEmissive +
        finalDiffuse * saturate(diffuse) +
        finalSpecular * saturate(specular);
    return float4(final.rgb, 1);
}
