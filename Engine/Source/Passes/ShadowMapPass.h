#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;

class Graphics;
class Texture;
class Scene;

_declspec(align(16)) struct ShadowMapConstant
{
    float mapWidth;
    float mapHeight;
    XMMATRIX lightViewProjection;
};

struct ShadowMapRenderDesc
{
    XMVECTOR position;
    XMVECTOR focus;
    XMVECTOR up = XMVectorSet(0, 1, 0, 1);
    XMMATRIX projection;
    XMMATRIX view;
    float textureWidth;
    float textureHeight;
    inline XMMATRIX CalculateView()
    {
        return XMMatrixLookAtLH(position, focus, up);
    }
};

class ShadowMapPass
{
public:
    ShadowMapPass(Graphics& graphics);
    void Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc);
private:
    std::unique_ptr<Shader> shader;
};
