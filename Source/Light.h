#pragma once
#include "stdafx.h"
#define MAX_LIGHTS 3

enum LightType
{
    DirectionalLight = 0,
    PointLight = 1,
    SpotLight = 2
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
    int m_Enabled;
};

_declspec(align(16)) struct LightProperties
{
    XMFLOAT4 m_EyePosition;
    XMFLOAT4 m_GlobalAmbient;
    Light m_Lights[MAX_LIGHTS];
};
