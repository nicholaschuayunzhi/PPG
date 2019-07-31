#define MAX_LIGHTS 3
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

Texture2D Diffuse : register(t0);
Texture2D NormalMap : register(t1);
Texture2D Specular : register(t2);
Texture2D ShadowMap : register(t3); // support 1 for now, future use array

sampler Sampler: register(s0);

cbuffer Material : register(b0)
{
    float4 matEmissive;
    float4 matAmbient;
    float4 matDiffuse;
    float4 matSpecular;

    float matShininess;
    int useDiffuse;
    int useNormal;
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

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);
    float3 N = normalize(IN.normal);
    float3 V = normalize(eyePosition.xyz - IN.wPosition.xyz);
    float3 L;
    float distance;
    LightingResult result;
    float attenuation;

    if (useNormal)
    {
        float3 calcN = cross(IN.binormal, IN.tangent);
        float3 bumpNormal = NormalMap.Sample(Sampler, IN.texCoord).xyz;
        bumpNormal = normalize((bumpNormal * 2) - 1.0);
        N = bumpNormal.x * IN.tangent + bumpNormal.y * IN.binormal + bumpNormal.z * IN.normal;
        N = normalize(N);
    }

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        if (light.status == LIGHT_DISABLED)
            continue;
        switch (abs(light.lightType))
        {
            case DIRECTIONAL_LIGHT:
                float lightFactor = (light.status == LIGHT_ENABLED_W_SHADOWMAP) ? 1 - ShadowFactor(IN.wPosition) : 1;
                L = -normalize(light.direction.xyz);
                result = CalculatePhongLighting(L, N, V, light.color, matShininess);
                diffuse += lightFactor * result.diffuse;
                specular += lightFactor * result.specular;
                break;
            case POINT_LIGHT:
                L = light.position.xyz - IN.wPosition.xyz;
                distance = length(L);
                L = normalize(L);
                result = CalculatePhongLighting(L, N, V, light.color, matShininess);
                attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadAtt * (distance * distance));
                diffuse += attenuation * result.diffuse;
                specular += attenuation * result.specular;
                break;
            case SPOT_LIGHT:
                L = light.position.xyz - IN.wPosition.xyz;
                distance = length(L);
                L = normalize(L);
                result = CalculatePhongLighting(L, N, V, light.color, matShininess);
                attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + light.quadAtt * (distance * distance));
                float minCos = cos(light.spotAngle);
                float maxCos = (minCos + 1.0f) / 2.0f; // squash between [0, 1]
                float cosAngle = dot(light.direction.xyz, -L);
                float intensity = smoothstep(minCos, maxCos, cosAngle);
                diffuse += intensity * attenuation * result.diffuse;
                specular += intensity * attenuation * result.specular;
                break;
            default:
                return float4(1, 0, 1, 1);
        }
    }

      float4 finalDiffuse = useDiffuse ? Diffuse.Sample(Sampler, IN.texCoord) : matDiffuse;
      float4 finalSpecular = useSpecular ? Specular.Sample(Sampler, IN.texCoord) : matSpecular;
      return matEmissive +
          matAmbient * saturate(globalAmbient) +
          finalDiffuse * saturate(diffuse) +
          finalSpecular * saturate(specular);
}
