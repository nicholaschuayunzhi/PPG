#pragma once
#include "stdafx.h"

using namespace DirectX;

class Camera
{
public:
    XMVECTOR m_eyePosition = XMVectorSet(0, 0, -10, 1);
    XMVECTOR m_focusPoint = XMVectorSet(0, 0, 0, 1);
    XMVECTOR m_upDirection = XMVectorSet(0, 1, 0, 0);
    XMMATRIX CalculateProjection(RECT clientRect);
    XMMATRIX CalculateView();
};
