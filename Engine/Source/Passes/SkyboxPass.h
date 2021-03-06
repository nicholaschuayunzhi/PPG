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
    SkyboxPass(Graphics& graphics, Texture& renderTarget, const LPCWSTR& fileName, float size = 50);
    void GenerateCubeMap(Graphics& graphics, Scene& scene);
    Texture* GenerateEnvMap(Graphics& graphics, Scene& scene);
    Texture* GenerateEnvPreFilter(Graphics& graphics, Scene& scene);
    Texture* GenerateBrdfLUT(Graphics& graphics, Scene& scene);
    std::unique_ptr<Texture> m_EnvironmentMap;
    std::unique_ptr<Texture> m_SpecularMap;
    std::unique_ptr<Texture> m_BrdfLUT;
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> rectToCubeMapShader;
    std::unique_ptr<Shader> cubeToEnvMapShader;
    std::unique_ptr<Shader> prefilterShader;
    std::unique_ptr<Shader> brdfIntegrateShader;
    std::unique_ptr<Mesh> skyboxMesh;
    std::unique_ptr<Texture> skyboxTexture;
    std::unique_ptr<Texture> skyboxCubeMap;
    XMMATRIX scaleMatrix;
    Texture& m_RenderTarget;
    bool m_CubeMapGenerationRequired = false;
    int m_CubeMapSize = 512;
};

