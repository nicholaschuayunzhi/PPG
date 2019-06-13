#include "stdafx.h"
#include "Camera.h"

XMMATRIX Camera::CalculateProjection(RECT clientRect)
{
    float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

    return XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 200.0f);
}

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
        // Weird locking at 90 degree angles.
        XMVECTOR viewRotation = XMQuaternionRotationRollPitchYaw(input.deltaMouseY, input.deltaMouseX, 0);
        m_Forward = XMVector3Normalize(XMVector3Rotate(m_Forward, viewRotation));
        m_Right = XMVector3Normalize(XMVector3Cross(m_Up, m_Forward));
    }
    m_EyePosition += ((z * m_Forward) + (y * m_Up) + (x * m_Right)) * m_Speed * deltaTime;
    m_LookAt = m_EyePosition + m_Forward;
}
