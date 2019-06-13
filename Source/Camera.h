#pragma once
#include "stdafx.h"
#include "Input.h"

using namespace DirectX;

class Camera
{
public:
    XMVECTOR m_EyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_Forward = XMVectorSet(0, 0, 1, 0);
    XMVECTOR m_Right = XMVectorSet(1, 0, 0, 0);
    XMVECTOR m_Up = XMVectorSet(0, 1, 0, 0);
    XMVECTOR m_LookAt = m_EyePosition + m_Forward;

    float m_Speed = 10;
    XMMATRIX CalculateProjection(RECT clientRect);
    XMMATRIX CalculateView();
    void HandleMovement(Input input, float deltaTime);
};
