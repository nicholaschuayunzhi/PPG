#define MAX_LIGHTS 1
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D Texture : register(t0);
sampler Sampler: register(s0);

struct Light
{
    float4 position;
    float4 direction;
    float4 color;

    int lightType;
    int enabled;
};

cbuffer LightProperties : register(b0)
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

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float matShininess = 10;
    float matDiffuse = globalAmbient;
    float matSpecular = globalAmbient;

    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);

    float3 N = normalize(IN.normal);
    float3 V = normalize(eyePosition.xyz - IN.wPosition.xyz);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        Light light = Lights[i];
        switch (abs(light.lightType))
        {
            case DIRECTIONAL_LIGHT:
                float3 L = -normalize(light.direction.xyz);
                diffuse += max(0, dot(N, L)) * light.color;
                float3 R = normalize(reflect(-L, N));
                float RdotV = max(0, dot(R, V));
                specular += light.color * pow(RdotV, matShininess);
                break;
            default:
                return float4(1, 0, 1, 1);
        }
    }
    float4 matColor =
        globalAmbient +
        matDiffuse * diffuse +
        matSpecular * specular;
    //float4 texColor = Texture.Sample(Sampler, IN.texCoord);
    return float4(matColor.xyz, 1);
}