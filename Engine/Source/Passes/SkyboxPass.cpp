#include "stdafx.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "SkyboxPass.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"

SkyboxPass::SkyboxPass(Graphics& graphics, Texture& renderTarget, const LPCWSTR& fileName, float size /*=50*/) :
    m_RenderTarget(renderTarget)
{
    std::vector<WORD> skyboxIndices =
    {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
       12, 13, 14, 12, 14, 15,
       16, 17, 18, 16, 18, 19,
       20, 21, 22, 20, 22, 23
    };

    skyboxMesh = std::make_unique<Mesh>(CubeVertices(), std::move(skyboxIndices), graphics);
    skyboxTexture = std::unique_ptr<Texture>(Texture::LoadTextureFromPath(graphics, fileName));
    shader = std::make_unique<Shader>(L"Skybox.vs.cso", L"Skybox.ps.cso", graphics);
    scaleMatrix = XMMatrixScaling(50, 50, 50);
}

void SkyboxPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.SetRenderTarget(m_RenderTarget);

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.UseModel(graphics);

    auto model = XMMatrixMultiply(scaleMatrix, XMMatrixTranslationFromVector(scene.m_MainCamera.m_EyePosition));
    shader->Use(deviceContext);
    scene.UpdateModel(graphics, model);
    skyboxTexture->UseSRV(deviceContext, 0);
    skyboxMesh->Draw(deviceContext);
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    deviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}