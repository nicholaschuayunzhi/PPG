#pragma once
#include "stdafx.h"
#include "Passes/ShadowMapPass.h"
#define MAX_LIGHTS 3

enum LightType
{
    DirectionalLight = 0,
    PointLight = 1,
    SpotLight = 2
};

enum LightStatus
{
    Disabled = 0,
    Enabled = 1,
    Static_Shadows = 2
};

using namespace DirectX;

_declspec(align(16)) struct Light
{
    XMFLOAT4 m_Position;
    XMFLOAT4 m_Direction;
    XMFLOAT4 m_Color;

    float m_ConstantAttenuation = 1.0f;
    float m_LinearAttenuation = 0.09f;
    float m_QuadAttenuation = 0.032f;
    float m_SpotAngle;

    int m_LightType;
    int m_Status = LightStatus::Disabled;
};

_declspec(align(16)) struct LightProperties
{
    XMFLOAT4 m_EyePosition;
    XMFLOAT4 m_GlobalAmbient;
    Light m_Lights[MAX_LIGHTS];
};

class Graphics;
class Texture;
class Scene;
class ForwardPass;

class LightManager
{
public:
    LightManager() = default;
    ~LightManager();
    void Update(Graphics& graphics);
    void Use(ID3D11DeviceContext* deviceContext, UINT slot = 0);
    Light& GetLight(unsigned int index);
    XMFLOAT4& GetEyePosition();
    LightManager& AddLight(Light& light);
    LightManager& SetGlobalAmbient(XMFLOAT4 colour);
    LightManager& SetEyePosition(XMVECTOR eyePositionVec);
    void SetLightWithShadows(Graphics& graphics, unsigned int index, ShadowMapRenderDesc& desc);
    unsigned int m_NumLights = 0;
    void RenderAnyShadowMap(Graphics& Graphics, Scene& scene);
private:
    ID3D11Buffer* m_Buffer;
    LightProperties m_LightProps;

    friend class ForwardPass;
    bool hasLightWithShadows = false;
    ID3D11Buffer* m_OneShadowMapCBuffer;
    ShadowMapRenderDesc m_OneShadowMapDesc;
    std::unique_ptr<Texture> m_OneShadowMapTexture;
    std::unique_ptr<ShadowMapPass> m_ShadowMapPass;
};
