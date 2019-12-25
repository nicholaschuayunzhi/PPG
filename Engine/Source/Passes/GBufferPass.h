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
    GBufferPass(Graphics& graphics, Texture& diffuse, Texture& specular, Texture& normals);
    ~GBufferPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    Texture& m_Diffuse;
    Texture& m_Specular;
    Texture& m_Normals;
    ID3D11Buffer* m_Buffer = nullptr;
    ID3D11RenderTargetView* m_RenderTargets[3];
};
