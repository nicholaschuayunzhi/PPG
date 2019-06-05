#pragma once
#include "stdafx.h"

using namespace DirectX;

class Camera
{
public:
    XMVECTOR m_EyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_LookAt = m_EyePosition + XMVectorSet(0, 0, 1, 0);
    XMVECTOR m_UpDirection = XMVectorSet(0, 1, 0, 0);
    float m_Speed = 10;
    XMMATRIX CalculateProjection(RECT clientRect);
    XMMATRIX CalculateView();
};
