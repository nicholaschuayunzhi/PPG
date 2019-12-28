#pragma once
#include "stdafx.h"
#include "Resources/Shader.h"

using namespace DirectX;
class Graphics;
class Scene;
class Texture;
class Sampler;

_declspec(align(16))
struct SSAOBuffer
{
    XMMATRIX m_ViewProjection;
    XMMATRIX m_InverseViewProjection;
    XMFLOAT4 m_Kernel[64];
    XMFLOAT4 m_CameraPosition;
    float m_Radius;
    XMFLOAT2 m_WindowSize;
};

class SSAOPass
{
public:
    SSAOPass(Graphics& graphics, Texture& renderTarget, Texture& depth, Texture& normals);
    ~SSAOPass();
    void Render(Graphics& graphics, Scene& scene);
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Noise;
    Texture& m_Depth;
    Texture& m_Normals;
    Texture& m_RenderTarget;
    ID3D11Buffer* m_Buffer = nullptr;
    SSAOBuffer m_SSAOBuffer;
};

