#pragma once
#include "stdafx.h"
#include "Input.h"

using namespace DirectX;

class Camera
{
public:
    const XMVECTOR FORWARD = XMVectorSet(0, 0, 1, 0);
    const XMVECTOR RIGHT = XMVectorSet(0, 0, 1, 0);
    const XMVECTOR UP = XMVectorSet(0, 1, 0, 0);

    XMVECTOR m_EyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_Forward = FORWARD;
    XMVECTOR m_Right = RIGHT;
    XMVECTOR m_LookAt = m_EyePosition + m_Forward;
    float m_Pitch = 0;
    float m_Yaw = 0;

    float m_Speed = 10;
    XMMATRIX CalculateProjection(RECT clientRect);
    XMMATRIX CalculateView();
    void HandleMovement(Input input, float deltaTime);
};
