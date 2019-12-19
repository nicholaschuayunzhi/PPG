#pragma once
#include "stdafx.h"
#include "LowLevel/Input.h"

using namespace DirectX;

class Graphics;

class Camera
{
public:
    Camera() = default;

    const XMVECTOR FORWARD = XMVectorSet(0, 0, 1, 0);
    const XMVECTOR RIGHT = XMVectorSet(1, 0, 0, 0);
    const XMVECTOR UP = XMVectorSet(0, 1, 0, 0);

    XMVECTOR m_EyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_Forward = FORWARD;
    XMVECTOR m_Right = RIGHT;
    XMVECTOR m_Up = UP;
    XMVECTOR m_LookAt = m_EyePosition + m_Forward;
    float m_Pitch = 0;
    float m_Yaw = 0;
    float m_NearZ = 0.1;
    float m_FarZ = 200;
    float m_Speed = 10;

    virtual XMMATRIX CalculateProjection() = 0;
    XMMATRIX CalculateView();
    void HandleMovement(Input input, float deltaTime);
 };

class PerspectiveCamera : public Camera
{
public:
    float m_AspectRatio = 1.777f;
    float m_FovY = 45;

    XMMATRIX CalculateProjection() override;
};

class OrthographicCamera : public Camera
{
public:
    float m_ViewWidth = 1;
    float m_ViewHeight = 1;

    XMMATRIX CalculateProjection() override;
};
