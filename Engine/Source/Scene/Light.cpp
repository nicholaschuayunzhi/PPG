#include "stdafx.h"
#include "Light.h"
#include "LowLevel/Graphics.h"
#include "Resources/Texture.h"
#include "Passes/ShadowMapPass.h"
#include "Resources/Shader.h"

LightManager::~LightManager()
{
    SafeRelease(m_Buffer);
    SafeRelease(m_OneShadowMapCBuffer);
}

void LightManager::Update(Graphics& graphics)
{
    if (m_Buffer == nullptr)
    {
        m_Buffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &m_LightProps);
    }
    else
    {
        graphics.UpdateBuffer(m_Buffer, &m_LightProps);
    }
}

LightManager& LightManager::AddLight(Light& light)
{
    if (m_NumLights < MAX_LIGHTS)
    {
        light.m_Status = LightStatus::Enabled;
        m_LightProps.m_Lights[m_NumLights] = light;
        m_NumLights++;
        return *this;
    }
    else
    {
        throw std::exception("LightManager:: Too many lights added");
    }
}

Light& LightManager::GetLight(unsigned int index)
{
    if (index < MAX_LIGHTS)
    {
        return m_LightProps.m_Lights[index];
    }
    else
    {
        throw std::exception("LightManager:: light index out of range");
    }
}


LightManager& LightManager::SetEyePosition(XMVECTOR eyePositionVec)
{
    XMStoreFloat4(&(m_LightProps.m_EyePosition), eyePositionVec);
    return *this;
}

LightManager& LightManager::SetGlobalAmbient(XMFLOAT4 colour)
{
    m_LightProps.m_GlobalAmbient = colour;
    return *this;
}

XMFLOAT4& LightManager::GetEyePosition()
{
    return m_LightProps.m_EyePosition;
}

void LightManager::Use(ID3D11DeviceContext* deviceContext, UINT slot /* = 0*/)
{
    deviceContext->PSSetConstantBuffers(slot, 1, &m_Buffer);
}

void LightManager::SetLightWithShadows(Graphics& graphics, unsigned int index, ShadowMapRenderDesc& desc)
{
    Light& light = GetLight(index);
    if (light.m_LightType != LightType::DirectionalLight)
    {
        throw std::exception("LightManager:: Only support static directional light");
    }

    if (hasLightWithShadows)
    {
        throw std::exception("LightManager:: Only support one shadow map");
    }

    light.m_Status = LightStatus::Static_Shadows;

    RECT& clientRect = graphics.m_ClientRect;
    ShadowMapConstant shadowMapConstant = {
         desc.textureWidth,
         desc.textureHeight,
         XMMatrixMultiply(desc.view, desc.projection)
    };
    m_OneShadowMapDesc = desc;
    m_OneShadowMapCBuffer = graphics.CreateBuffer(sizeof(ShadowMapConstant), D3D11_BIND_CONSTANT_BUFFER, &shadowMapConstant);
    m_ShadowMapPass = std::make_unique<ShadowMapPass>(graphics);
    m_OneShadowMapTexture = std::make_unique<Texture>(desc.textureWidth, desc.textureHeight, graphics, "Shadow Map");
    hasLightWithShadows = true;
}

void LightManager::RenderAnyShadowMap(Graphics& graphics, Scene& scene)
{
    if (hasLightWithShadows)
    {
        m_ShadowMapPass->Render(graphics, scene, *m_OneShadowMapTexture, m_OneShadowMapDesc);
    }
}
