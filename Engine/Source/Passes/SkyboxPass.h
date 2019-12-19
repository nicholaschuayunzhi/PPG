#pragma once
#include "stdafx.h"

class Texture;
class Mesh;
class Shader;
class Graphics;
class Scene;

class SkyboxPass
{
public:
    SkyboxPass(Graphics& graphics, Texture& renderTarget, LPCWSTR fileName, float size = 50);
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> skyboxMesh;
    std::unique_ptr<Texture> skyboxTexture;
    XMMATRIX scaleMatrix;
    Texture& m_RenderTarget;
};
