#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;

class ForwardPass
{
public:
    ForwardPass(Graphics& graphics);
    ~ForwardPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    ID3D11Buffer* m_Buffer = nullptr;
};
