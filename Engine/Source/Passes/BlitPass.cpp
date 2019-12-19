#include "stdafx.h"
#include "BlitPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"

BlitPass::BlitPass(Graphics& graphics, Texture& src, Texture& dest) :
    m_SrcTexture(src),
    m_DestTexture(dest)
{
    m_Shader = std::make_unique<Shader>(L"VertexShader.cso", L"Blit.ps.cso", graphics);
    m_Mesh = std::make_unique<Mesh>(QuadVertices(), QuadIndices(), graphics);
    m_Sampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
    m_Camera.m_EyePosition = XMVectorSet(0, 1, 0, 1);
    m_Camera.m_Up = XMVectorSet(0, 0, 1, 1);
    m_Camera.m_Right = XMVectorSet(1, 0, 0, 1);
    m_Camera.m_LookAt = XMVectorSet(0, 0, 0, 1);
    m_Camera.m_ViewWidth = 2;
    m_Camera.m_ViewHeight = 2;
}

BlitPass::~BlitPass()
{
}

void BlitPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    graphics.SetRenderTarget(m_DestTexture);
    scene.UseCamera(graphics, m_Camera);
    scene.UseModel(graphics);
    scene.UpdateModel(graphics, XMMatrixIdentity());
    m_Sampler->Use(deviceContext, 0);
    m_SrcTexture.Use(deviceContext, 0);
    m_Shader->Use(deviceContext);
    m_Mesh->Draw(deviceContext);
}
