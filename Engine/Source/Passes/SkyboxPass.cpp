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
    scaleMatrix = XMMatrixScaling(size, size, size);
    shader = std::make_unique<Shader>(L"Skybox.vs.cso", L"Skybox.ps.cso", graphics);

    WCHAR ext[_MAX_EXT];
    _wsplitpath_s(fileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    if (_wcsicmp(ext, L".dds") == 0)
    {
        skyboxCubeMap = std::unique_ptr<Texture>(Texture::LoadTextureFromPath(graphics, fileName));
        m_CubeMapGenerationRequired = false;
    }
    else
    {
        skyboxTexture = std::unique_ptr<Texture>(Texture::LoadTextureFromPath(graphics, fileName));
        rectToCubeMapShader = std::make_unique<Shader>(L"Skybox.vs.cso", L"RectToCubeMap.ps.cso", graphics);
        skyboxCubeMap = std::unique_ptr<Texture>(Texture::CreateTextureCube(
            graphics, m_CubeMapSize, "Cube Map", DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE));
        skyboxCubeMap->CreateSRV(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_SRV_DIMENSION_TEXTURECUBE);
        skyboxCubeMap->CreateTextureCubeRTVs(graphics, DXGI_FORMAT_R16G16B16A16_FLOAT);
        m_CubeMapGenerationRequired = true;
    }
}

void SkyboxPass::GenerateCubeMap(Graphics& graphics, Scene& scene)
{
    if (!m_CubeMapGenerationRequired)
    {
        return;
    }

    XMVECTOR forward[6] =
    {
        { 1, 0, 0, 0 },
        { -1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, -1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, -1, 0 },
    };

    XMVECTOR up[6] =
    {
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, -1, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 0,  0 },
        { 0, 1, 0, 0 },
    };

    auto deviceContext = graphics.m_DeviceContext;
    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = m_CubeMapSize;
    viewport.Height = m_CubeMapSize;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &viewport);

    for (int i = 0; i < 6; ++i)
    {
        graphics.SetRenderTarget(skyboxCubeMap->GetRTV(i), false);
        OrthographicCamera ortho;
        ortho.m_EyePosition = XMVectorSet(0, 0, 0, 0);
        ortho.m_LookAt = forward[i];
        ortho.m_Up = up[i];
        ortho.m_NearZ = 0;
        ortho.m_FarZ = 10;
        ortho.m_ViewHeight = 2;
        ortho.m_ViewWidth = 2;
        rectToCubeMapShader->Use(deviceContext);
        skyboxTexture->UseSRV(deviceContext, 0);
        scene.UseModel(graphics);
        scene.UseCamera(graphics, ortho);
        scene.UpdateModel(graphics, XMMatrixIdentity());

        skyboxMesh->Draw(deviceContext);
    }

    graphics.SetUp();
    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
    m_CubeMapGenerationRequired = false;
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
    skyboxCubeMap->UseSRV(deviceContext, 0);
    skyboxMesh->Draw(deviceContext);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindRenderTargetView();
}