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
        0,  2,  1,  0,  3,  2,
        4,  6,  5,  4,  7,  6,
        8,  10, 9,  8, 11, 10,
       12, 14, 13, 12, 15, 14,
       16, 18, 17, 16, 19, 18,
       20, 22, 21, 20, 23, 22
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