#pragma once
#include "stdafx.h"
#include "Scene.h"

class Texture;
class Mesh;
class Shader;
class Graphics;

class SkyboxPass
{
public:
    SkyboxPass(Graphics& graphics, LPCWSTR fileName, float size = 50);
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> skyboxMesh;
    std::unique_ptr<Texture> skyboxTexture;
    XMMATRIX scaleMatrix;
};
