#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::~Camera()
{
    SafeRelease(m_ViewBuffer);
    SafeRelease(m_ProjBuffer);
}

XMMATRIX Camera::CalculateProjection(RECT clientRect)
{
    float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

    return XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 200.0f);
}

XMMATRIX Camera::CalculateView()
{
    return XMMatrixLookAtLH(m_EyePosition, m_LookAt, UP);
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

void Camera::UpdateView(Graphics& graphics, XMMATRIX view)
{
    if (m_ViewBuffer == nullptr)
    {
        m_ViewBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &view);
    }
    else
    {
        graphics.UpdateBuffer(m_ViewBuffer, &view);
    }
}

void Camera::UpdateProjection(Graphics& graphics, XMMATRIX proj)
{
    if (m_ProjBuffer == nullptr)
    {
        m_ProjBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &proj);
    }
    else
    {
        graphics.UpdateBuffer(m_ProjBuffer, &proj);
    }
}

void Camera::Use(ID3D11DeviceContext* deviceContext)
{
    deviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
    deviceContext->VSSetConstantBuffers(2, 1, &m_ProjBuffer);
}
