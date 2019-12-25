#define MAX_LIGHTS 3
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

#define VERTEX_NORMALS 0
#define NORMAL_MAP 1
#define BUMP_MAP 2

Texture2D Diffuse : register(t0);
Texture2D NormalMap : register(t1);
Texture2D Specular : register(t2);
Texture2D ShadowMap : register(t4); // support 1 for now, future use array

sampler Sampler : register(s0);

cbuffer Material : register(b0)
{
    float4 matEmissive;
    float4 matDiffuse;
    float4 matSpecular;

    float matShininess;
    int useDiffuse;
    int normalState;
    int useSpecular;
}

struct Light
{
    float4 position;
    float4 direction;
    float4 color;

    float constantAtt;
    float linearAtt;
    float quadAtt;
    float spotAngle;

    int lightType;
    int status;
};

cbuffer LightProperties : register(b1)
{
    float4 eyePosition;
    float4 globalAmbient;
    Light Lights[MAX_LIGHTS];
}

cbuffer ShadowMapConstants : register(b2) // supports one
{
    float mapWidth;
    float mapHeight;
    float4x4 lightViewProjection;
}

struct LightingResult
{
    float4 diffuse;
    float4 specular;
};

float ShadowFactor(float4 worldPosition) // assumes only one shadow map cbuffer
{
    float4 lightSpacePosition = mul(lightViewProjection, worldPosition);

    float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    float currentDepth = projCoords.z;

    if (currentDepth > 1)
        return 0;

    projCoords = (projCoords + 1) / 2.0; // change to [0 - 1]
    projCoords.y = -projCoords.y; // bottom right corner is (1, -1) in NDC so we have to flip it

    float2 texelSize = float2(1, 1) / float2(mapWidth, mapHeight);

    float shadow = 0;
    float epsilon = 0.01f;
    for (int x = -1; x < 2; ++x)
        for (int y = -1; y < 2; ++y)
        {
            float closestDepth = ShadowMap.Sample(Sampler, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += (closestDepth < currentDepth - epsilon);
        }
    shadow /= 9;
    return shadow;
}

LightingResult CalculatePhongLighting(float3 L, float3 N, float3 V, float4 lightColor, float matShininess)
{
    LightingResult result;
    result.diffuse = max(0, dot(N, L)) * lightColor;
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    result.specular = lightColor * pow(RdotV, matShininess);
    return result;
}

float3 CalculateNormal(float3 normal, float3 tangent, float3 binormal, float2 texCoord)
{
    float3 N = normal;
    switch (normalState)
    {
        case NORMAL_MAP:
            float3 normalMapVal = NormalMap.Sample(Sampler, texCoord).xyz;
            normalMapVal = normalize((normalMapVal * 2) - 1.0);
            N = (normalMapVal.x * normalize(tangent)) + (normalMapVal.y * normalize(binormal)) + (normalMapVal.z * normal);
            N = normalize(N);
            return N;
            break;
        case BUMP_MAP:
            float2 bumpTextureSize;
            NormalMap.GetDimensions(bumpTextureSize[0], bumpTextureSize[1]);
            float2 pixelSize = 1.0 / bumpTextureSize;

            float mid = NormalMap.Sample(Sampler, texCoord) * 2.0 - 1.0;
            float left = NormalMap.Sample(Sampler, texCoord + float2(-pixelSize.x, 0)) * 2.0 - 1.0;
            float right = NormalMap.Sample(Sampler, texCoord + float2(pixelSize.x, 0)) * 2.0 - 1.0;
            float top = NormalMap.Sample(Sampler, texCoord + float2(0, -pixelSize.y)) * 2.0 - 1.0;
            float bottom = NormalMap.Sample(Sampler, texCoord + float2(0, pixelSize.y)) * 2.0 - 1.0;
            float3 p1 = ((bottom - mid) - (top - mid)) * normalize(binormal);
            float3 p2 = ((left - mid) - (right - mid)) * normalize(tangent);
            N = normalize(N - (p1 + p1));
            return N;
            break;
    }
    return normal;
}

struct ShadingInfo
{
    float4 posW;
    float3 normal;
    float3 viewDir;
};

void CalculatePointLight(Light light, ShadingInfo shadingInfo, inout float4 diffuse, inout float4 specular)
{
    float3 L = light.position.xyz - shadingInfo.posW.xyz;
    float3 N = shadingInfo.normal;
    float3 V = shadingInfo.viewDir;
    float distance = length(L);
    L = normalize(L);
    LightingResult result = CalculatePhongLighting(L, N, V, light.color, matShininess);
    float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadAtt * (distance * distance));
    diffuse += attenuation * result.diffuse;
    specular += attenuation * result.specular;
}

void CalculateDirectionalLight(Light light, ShadingInfo shadingInfo, inout float4 diffuse, inout float4 specular)
{
    float3 L = -normalize(light.direction.xyz);
    float3 N = shadingInfo.normal;
    float3 V = shadingInfo.viewDir;

    float lightFactor = (light.status == LIGHT_ENABLED_W_SHADOWMAP) ? 1 - ShadowFactor(shadingInfo.posW) : 1;
    LightingResult result = CalculatePhongLighting(L, N, V, light.color, matShininess);
    diffuse += lightFactor * result.diffuse;
    specular += lightFactor * result.specular;
}

void CalculateSpotLight(Light light, ShadingInfo shadingInfo, inout float4 diffuse, inout float4 specular)
{
    float3 L = light.position.xyz - shadingInfo.posW.xyz;
    float3 N = shadingInfo.normal;
    float3 V = shadingInfo.viewDir;
    float distance = length(L);
    L = normalize(L);

    LightingResult result = CalculatePhongLighting(L, N, V, light.color, matShininess);
    float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadAtt * (distance * distance));
    float minCos = cos(light.spotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f; // squash between [0, 1]
    float cosAngle = dot(light.direction.xyz, -L);
    float intensity = smoothstep(minCos, maxCos, cosAngle);
    diffuse += intensity * attenuation * result.diffuse;
    specular += intensity * attenuation * result.specular;
}
