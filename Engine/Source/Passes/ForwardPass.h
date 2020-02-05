#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;

class ForwardPass
{
public:
    ForwardPass(Graphics& graphics, Texture& renderTarget);
    ~ForwardPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    Texture& m_RenderTarget;
    ID3D11Buffer* m_Buffer = nullptr;
    ID3D11Buffer* m_BoneBuffer = nullptr;
};
