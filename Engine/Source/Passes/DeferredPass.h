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
    DeferredPass(Graphics& graphics, Texture& renderTarget, Texture& diffuse, Texture& specular, Texture& normals);
    ~DeferredPass();
    void UseAmbientOcclusion(Texture& aoMap);
    void DisableAmbientOcclusion();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> m_Shader;
    Texture& m_Diffuse;
    Texture& m_Specular;
    Texture& m_Normals;
    Texture& m_RenderTarget;
    Texture* m_AO;
    bool m_UseAO = false;
    ID3D11Buffer* m_Buffer = nullptr;
};
