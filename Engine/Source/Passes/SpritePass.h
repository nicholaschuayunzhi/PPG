#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"
#include "Resources/Mesh.h"
#include "Scene/Camera.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

class SpritePass
{
public:
    SpritePass(Graphics& graphics, Texture& renderTarget);
    ~SpritePass();
    void Render(Graphics& graphics, Scene& scene);
private:
    Texture& m_RenderTarget;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Mesh> m_QuadMesh;
    std::unique_ptr<Sampler> m_Sampler;
    ID3D11DepthStencilState* m_NoWriteDepthStencil;
    OrthographicCamera m_Camera;
};


