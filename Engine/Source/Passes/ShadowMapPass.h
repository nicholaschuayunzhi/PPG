#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"
#include "Scene/Camera.h"

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
    XMVECTOR m_EyePosition;
    XMVECTOR m_LookAt;
    float m_NearZ = 0.1;
    float m_FarZ = 200;
    float m_ViewWidth = 1;
    float m_ViewHeight = 1;
    float m_TextureWidth;
    float m_TextureHeight;
};

class ShadowMapPass
{
public:
    ShadowMapPass(Graphics& graphics);
    void Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc);
private:
    std::unique_ptr<Shader> shader;
};
