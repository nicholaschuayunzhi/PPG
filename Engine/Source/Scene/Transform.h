#pragma once
#include "stdafx.h"
using namespace DirectX;

class Graphics;

struct Transform // try new code style
{
public:
    Transform();
    ~Transform() = default;
    Transform(Transform& t) = default;
    Transform(Transform&& t) = default;
    XMVECTOR position;
    XMVECTOR rotation;
    float scale;
    Transform& SetPosition(float x, float y, float z);
    Transform& Translate(float x, float y, float z);
    Transform& RotateEulerAngles(float x, float y, float z);
    Transform& UniformScale(float s);
    void Update(Graphics& graphics, ID3D11Buffer* buffer);
    XMMATRIX GetModel();
    XMMATRIX GetLocalModel();
private:
    friend class Scene;
    void SetModel(XMMATRIX model);
    float dirty;
    XMMATRIX model;
    XMMATRIX localModel;
};
