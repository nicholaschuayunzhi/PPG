#include "stdafx.h"
#include "Camera.h"

XMMATRIX Camera::CalculateProjection(RECT clientRect)
{
    float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

    return XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);
}

XMMATRIX Camera::CalculateView()
{
    return XMMatrixLookAtLH(m_EyePosition, m_LookAt, m_UpDirection);
}

void Camera::HandleMovement(Input input, float deltaTime)
{
    XMVECTOR viewRotation = XMQuaternionRotationRollPitchYaw(input.mouseY, input.mouseX, 0);
    if (input.key != Key::NONE)
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        if (input.key & Key::FRONT) z += 1.0f;
        if (input.key & Key::BACK) z -= 1.0f;
        if (input.key & Key::LEFT) x -= 1.0f;
        if (input.key & Key::RIGHT) x += 1.0f;
        if (input.key & Key::UP) y += 1.0f;
        if (input.key & Key::DOWN) y -= 1.0f;
        m_EyePosition += XMVector3Rotate(XMVectorSet(x, y, z, 0), viewRotation) * m_Speed * deltaTime;
    }
    m_Forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), viewRotation);
    m_LookAt = m_EyePosition + m_Forward;
}
