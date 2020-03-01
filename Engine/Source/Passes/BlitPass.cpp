#include "stdafx.h"
#include "BlitPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

BlitPass::BlitPass(Graphics& graphics, Texture& src, Texture& dest) :
    m_SrcTexture(src),
    m_DestTexture(dest)
{
    m_Shader = std::make_unique<Shader>(L"Fullscreen.vs.cso", L"Blit.ps.cso", graphics);
}

BlitPass::~BlitPass()
{
}

void BlitPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    graphics.SetRenderTarget(m_DestTexture, false);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_SrcTexture.UseSRV(deviceContext, 0);
    m_Shader->Use(deviceContext);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}