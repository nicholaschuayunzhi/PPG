#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;

class ToneMapPass
{
public:
    ToneMapPass(Graphics& graphics, Texture& colour, Texture& dest);
    ~ToneMapPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    Texture& m_ColourTexture;
    Texture& m_DestTexture;
    std::unique_ptr<Shader> m_Shader;
};

