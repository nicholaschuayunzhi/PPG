#include "Common/Sampler.hlsli"
#include "Common/Lighting.hlsli"
#include "Common/Shading.hlsli"
#include "Common/BRDF.hlsli"

struct PixelShaderInput // see Fullscreen.vs.hlsl
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

cbuffer DeferredCBuffer : register(b3)
{
    float4x4 inverseProjection;
    float4x4 inverseView;
    int useAO;
}

Texture2D DepthTexture : register(t0);
Texture2D Diffuse : register(t1);
Texture2D MetalRough : register(t2);
Texture2D Normals : register(t3);
Texture2D AO : register(t5);

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

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float depth = DepthTexture.Sample(PointSampler, IN.texCoord).r;
    float3 posW = CalculateWorldFromDepth(depth, IN.texCoord);
    float4 matMetalRough = MetalRough.Sample(PointSampler, IN.texCoord);
    float3 albedo = Diffuse.Sample(PointSampler, IN.texCoord).rgb;
    float metallic = matMetalRough.r;
    float roughness = matMetalRough.g;
    float3 normal = Normals.Sample(PointSampler, IN.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    SurfaceInfo surf;
    surf.posW = float4(posW, 1);
    surf.N = normal;
    surf.V = normalize(eyePosition.xyz - posW);
    surf.NdotV = dot(surf.N, surf.V);

    float3 Lo = float3(0, 0, 0);
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        LightingInfo li = EvalLightingInfo(surf, light);

        // cook-torrance brdf
        float NDF = DistributionGGX(max(0.0, li.NdotH), roughness);
        float G = GeometrySmith(max(0.0, surf.NdotV), max(0.0, li.NdotL), roughness);
        float3 F = fresnelSchlick(clamp(surf.NdotV, 0.0, 1.0), F0);
        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(li.NdotL, 0.0); // clamped n dot l

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(surf.NdotV, 0.0) * NdotL;
        float3 specular = numerator / max(denominator, 0.001);

        Lo += (kD * albedo / PI + specular) * light.color.rgb * li.attenuation * NdotL * (li.shadowFactor);
    }
    float ao = useAO ? AO.Sample(PointSampler, IN.texCoord).r : 1.0;
    float3 ambient = globalAmbient.rgb * albedo * ao;
    float3 colour = ambient + Lo;

    return float4(colour, 1.0);
}
