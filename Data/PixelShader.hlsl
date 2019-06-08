#define MAX_LIGHTS 3
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D Texture : register(t0);
sampler Sampler: register(s0);

cbuffer Material : register(b0)
{
    float4 matEmissive;
    float4 matAmbient;
    float4 matDiffuse;
    float4 matSpecular;

    float matShininess;
    int useTexture;
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
    int enabled;
};

cbuffer LightProperties : register(b1)
{
    float4 eyePosition;
    float4 globalAmbient;
    Light Lights[MAX_LIGHTS];
}

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 wPosition: POSITION;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct LightingResult
{
    float4 diffuse;
    float4 specular;
};

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

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        switch (abs(light.lightType))
        {
            case DIRECTIONAL_LIGHT:
                L = -normalize(light.direction.xyz);
                result = CalculatePhongLighting(L, N, V, light.color, matShininess);
                diffuse += result.diffuse;
                specular += result.specular;
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
    float4 color =
        matEmissive +
        matAmbient * globalAmbient +
        matDiffuse * saturate(diffuse) +
        matSpecular * saturate(specular);

    if (useTexture)
    {
        color *= Texture.Sample(Sampler, IN.texCoord);
    }
    return color;
}