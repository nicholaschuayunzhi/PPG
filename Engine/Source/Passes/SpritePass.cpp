#include "stdafx.h"
#include "SpritePass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"


SpritePass::SpritePass(Graphics& graphics, Texture& renderTarget) :
    m_RenderTarget(renderTarget)
{
    m_Shader = std::make_unique<Shader>(L"VertexShader.cso", L"Sprite.ps.cso", graphics);
    m_QuadMesh = std::make_unique<Mesh>(QuadVertices(), QuadIndices(), graphics);
    m_Sampler = std::make_unique<Sampler>(graphics);

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;
    HRESULT hr = graphics.m_Device->CreateDepthStencilState(&depthStencilStateDesc, &m_NoWriteDepthStencil);

    if (FAILED(hr))
    {
        throw std::exception("Faied to create Depth stencil state in Sprite Pass");
    }
}

SpritePass::~SpritePass()
{
    SafeRelease(m_NoWriteDepthStencil);
}

void SpritePass::Render(Graphics& graphics, Scene& scene)
{
    graphics.SetRenderTarget(m_RenderTarget);
    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.UseModel(graphics);
    m_Shader->Use(deviceContext);
    m_Sampler->Use(deviceContext, 0);

    deviceContext->OMSetDepthStencilState(m_NoWriteDepthStencil, 1);
    
    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_SpriteRenderer.m_IsEnabled) continue;
        SpriteRenderer& spriteRenderer = sceneObj->m_SpriteRenderer;
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        spriteRenderer.m_Sprite->Use(deviceContext, 0);
        m_QuadMesh->Draw(deviceContext);
    }

    deviceContext->OMSetDepthStencilState(graphics.m_DepthStencilState, 1);
}
