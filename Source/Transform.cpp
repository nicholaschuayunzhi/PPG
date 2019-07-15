#include "stdafx.h"
#include "Transform.h"
#include "Graphics.h"

Transform::Transform() :
    model(XMMatrixIdentity()),
    position(XMVectorZero()),
    rotation(XMVectorZero()),
    scale(0)
{
    dirty = true;
}

Transform& Transform::SetPosition(float x, float y, float z)
{
    dirty = true;
    position = XMVectorSet(x, y, z, 0);
    return *this;
}


Transform& Transform::Translate(float x, float y, float z)
{
    dirty = true;
    position = XMVectorAdd(position, XMVectorSet(x, y, z, 0));
    return *this;
}

Transform& Transform::RotateEulerAngles(float x, float y, float z)
{
    dirty = true;
    rotation = XMVectorAdd(rotation, XMVectorSet(x, y, z, 0));
    return *this;
}

Transform& Transform::UniformScale(float s)
{
    dirty = true;
    scale = s;
    return *this;
}

void Transform::Update(Graphics& graphics, ID3D11Buffer* buffer)
{
    graphics.UpdateBuffer(buffer, &GetModel());
}

XMMATRIX Transform::GetModel()
{
    if (dirty)
    {
        model = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale),
            XMMatrixMultiply(XMMatrixRotationRollPitchYawFromVector(rotation),
                XMMatrixTranslationFromVector(position)));
        dirty = false;
    }
    return model;
}
