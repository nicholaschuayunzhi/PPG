#include "Common/Sampler.hlsli"
#include "Common/Lighting.hlsli"

struct PixelShaderInput // see Fullscreen.vs.hlsl
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer DeferredCBuffer : register(b3)
{
    float4x4 inverseProjection;
    float4x4 inverseView;
}

Texture2D DepthTexture : register(t0);
Texture2D Diffuse : register(t1);
Texture2D Specular : register(t2);
Texture2D Normals : register(t3);

float3 CalculateWorldFromDepth(float depth, float2 texCoord)
{
    // clip space between [-1, 1]
    // flip y so that +ve y is upwards
    float2 clipXY = texCoord * 2.0 - 1.0;
    clipXY.y = -clipXY.y;

    // NOTE: depth is not linearized
    // Also in range [0, 1] due to DirectX Convention
    float4 clipSpace = float4(clipXY, depth, 1);
    float4 viewSpace = mul(inverseProjection, clipSpace);

    // perspective divide
    viewSpace /= viewSpace.w;

    float4 worldSpace = mul(inverseView, viewSpace);
    return worldSpace.xyz;
}

static const float2 SpecPowerRange = { 0.0, 250.0 };

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float depth = DepthTexture.Sample(PointSampler, IN.texCoord).r;
    float3 posW = CalculateWorldFromDepth(depth, IN.texCoord);
    float4 matDiffuse = float4(Diffuse.Sample(PointSampler, IN.texCoord).rgb, 1);
    float4 matSpecularAndPow = Specular.Sample(PointSampler, IN.texCoord);
    float4 matSpecular = float4(matSpecularAndPow.rgb, 1);
    float specPow = matSpecularAndPow.a * SpecPowerRange.y + SpecPowerRange.x;
    float3 normal = Normals.Sample(PointSampler, IN.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    ShadingInfo shadingInfo;
    shadingInfo.posW = float4(posW, 1);
    shadingInfo.normal = normal;
    shadingInfo.viewDir = normalize(eyePosition.xyz - posW);
    shadingInfo.matShininess = specPow;

    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);

    int count = 0;
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        count++;
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
    float4 finalColour = saturate(diffuse) * matDiffuse + saturate(specular) * matSpecular;
    return float4(finalColour);

}
