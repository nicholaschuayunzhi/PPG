#include "stdafx.h"
#include "Light.h"
#include "Graphics.h"

LightManager::~LightManager()
{
    SafeRelease(m_Buffer);
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
        light.m_Enabled = 1;
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

