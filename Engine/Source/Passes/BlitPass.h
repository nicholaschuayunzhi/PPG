#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"
#include "Scene/Camera.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

class BlitPass
{
public:
    BlitPass(Graphics& graphics, Texture& src, Texture& dest);
    ~BlitPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    Texture& m_SrcTexture;
    Texture& m_DestTexture;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Sampler> m_Sampler;
    OrthographicCamera m_Camera;
};

