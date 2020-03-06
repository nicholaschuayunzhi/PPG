#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

class DeferredPass
{
public:
    DeferredPass(Graphics& graphics, Texture& renderTarget, Texture& diffuse, Texture& metalRough, Texture& normals, Texture& emissive);
    ~DeferredPass();
    void UseAmbientOcclusion(Texture& aoMap);
    void UseEnvMap(Texture* envMap, Texture* preFilter, Texture* brdfLut);
    void DisableAmbientOcclusion();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> m_Shader;
    Texture& m_Diffuse;
    Texture& m_MetalRough;
    Texture& m_Normals;
    Texture& m_Emissive;
    Texture& m_RenderTarget;
    Texture* m_AO;
    Texture* m_EnvMap;
    Texture* m_PreFilter;
    Texture* m_BrdfLut;
    bool m_UseAO = false;
    bool m_UseEnvMap = false;
    ID3D11Buffer* m_Buffer = nullptr;
};
