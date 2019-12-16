#include "stdafx.h"
#include "ShadowMapPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ShadowMapPass::ShadowMapPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"ShadowMap.ps.cso", graphics);
}

void ShadowMapPass::Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc)
{
    XMMATRIX view = XMMatrixLookAtLH(desc.position, desc.focus, desc.up);
    auto& camera = scene.camera;
    camera.UpdateProjection(graphics, desc.projection);
    camera.UpdateView(graphics, view);

    auto deviceContext = graphics.m_DeviceContext;
    scene.camera.Use(deviceContext);
    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);
    deviceContext->OMSetRenderTargets(1, &(shadowMapTexture.m_TextureRTV), graphics.m_DepthStencilView);
    _Graphics = &graphics;
    _Scene = &scene;
    for (auto sceneObj : scene.rootNode->m_Children) // root node is empty
        DrawSceneRecursive(*sceneObj, XMMatrixIdentity());
    ID3D11RenderTargetView* pNullRTV = NULL;
    deviceContext->OMSetRenderTargets(1, &(graphics.m_RenderTargetView), graphics.m_DepthStencilView);
    graphics.Clear(DirectX::Colors::Transparent, 1.0f, 0);
}

void ShadowMapPass::DrawSceneRecursive(SceneObject& obj, XMMATRIX model)
{
    model = XMMatrixMultiply(obj.m_Transform.GetModel(), model);
    _Scene->UpdateModel(*_Graphics, model);

    auto devCon = _Graphics->m_DeviceContext;
    obj.m_Mesh->Draw(devCon);

    for (auto child : obj.m_Children)
    {
        DrawSceneRecursive(*child, model);
    }
}
