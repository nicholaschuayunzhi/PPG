#include "stdafx.h"
#include "Camera.h"
#include "LowLevel/Graphics.h"


XMMATRIX Camera::CalculateView()
{
    return XMMatrixLookAtLH(m_EyePosition, m_LookAt, m_Up);
}

void Camera::HandleMovement(Input input, float deltaTime)
{
    float x = 0.0f, y = 0.0f, z = 0.0f;

    if (input.key != Key::NONE)
    {
        if (input.key & Key::FRONT) z += 1.0f;
        if (input.key & Key::BACK) z -= 1.0f;
        if (input.key & Key::LEFT) x -= 1.0f;
        if (input.key & Key::RIGHT) x += 1.0f;
        if (input.key & Key::UP) y += 1.0f;
        if (input.key & Key::DOWN) y -= 1.0f;
    }
    if (input.mouse & Mouse::RMB_DOWN)
    {
        m_Pitch += input.deltaMouseY;
        m_Yaw += input.deltaMouseX;
        XMVECTOR viewRotation = XMQuaternionRotationRollPitchYaw(m_Pitch, m_Yaw, 0);
        m_Forward = XMVector3Normalize(XMVector3Rotate(FORWARD, viewRotation));
        m_Right = XMVector3Normalize(XMVector3Cross(UP, m_Forward));
    }

    m_EyePosition += ((z * m_Forward) + (y * UP) + (x * m_Right)) * m_Speed * deltaTime;
    m_LookAt = m_EyePosition + m_Forward;
}


XMMATRIX PerspectiveCamera::CalculateProjection()
{
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FovY), m_AspectRatio, 0.1f, 200.0f);
}

XMMATRIX OrthographicCamera::CalculateProjection()
{
    return XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_NearZ, m_FarZ);
}
