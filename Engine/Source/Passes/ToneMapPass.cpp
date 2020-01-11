#include "stdafx.h"
#include "ToneMapPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ToneMapPass::ToneMapPass(Graphics& graphics, Texture& colour, Texture& dest) :
    m_ColourTexture(colour),
    m_DestTexture(dest)
{
    // currently only Reinhard tonemapping
    // future: ACES, Hable
    // https://64.github.io/tonemapping/
    // https://github.com/NVIDIAGameWorks/Falcor/blob/master/Source/Falcor/Data/Effects/ToneMapping.ps.slang
    m_Shader = std::make_unique<Shader>(L"Fullscreen.vs.cso", L"ToneMapReinhard.ps.cso", graphics);
}

ToneMapPass::~ToneMapPass()
{
}

void ToneMapPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    graphics.SetRenderTarget(m_DestTexture, false);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_ColourTexture.UseSRV(deviceContext, 0);
    m_Shader->Use(deviceContext);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}
