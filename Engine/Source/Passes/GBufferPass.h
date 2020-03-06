#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;

class GBufferPass
{
public:
    GBufferPass(Graphics& graphics, Texture& diffuse, Texture& metalRough, Texture& normals, Texture& emissive);
    ~GBufferPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    static const UINT NUM_RENDER_TARGETS = 4;
    std::unique_ptr<Shader> shader;
    Texture& m_Diffuse;
    Texture& m_MetalRough;
    Texture& m_Normals;
    Texture& m_Emissive;
    ID3D11Buffer* m_Buffer = nullptr;
    ID3D11Buffer* m_BoneBuffer = nullptr;
    ID3D11RenderTargetView* m_RenderTargets[NUM_RENDER_TARGETS];
};
