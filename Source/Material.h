#pragma once
#include "stdafx.h"

using namespace DirectX;

_declspec(align(16)) struct Material
{
    XMFLOAT4 m_Emissive;
    XMFLOAT4 m_Ambient;
    XMFLOAT4 m_Diffuse;
    XMFLOAT4 m_Specular;

    float m_Shininess;
    int m_UseTexture;
    int m_UseNormalMap;
};
