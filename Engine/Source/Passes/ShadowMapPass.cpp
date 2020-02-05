#include "stdafx.h"
#include "ShadowMapPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ShadowMapPass::ShadowMapPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"ShadowMap.ps.cso", graphics);
}

void ShadowMapPass::Render(Graphics& graphics, Scene& scene, Texture& shadowMapTexture, ShadowMapRenderDesc& desc)
{
    OrthographicCamera shadowMapCamera;
    shadowMapCamera.m_EyePosition = desc.m_EyePosition;
    shadowMapCamera.m_LookAt = desc.m_LookAt;
    shadowMapCamera.m_NearZ = desc.m_NearZ;
    shadowMapCamera.m_FarZ = desc.m_FarZ;
    shadowMapCamera.m_ViewHeight = desc.m_ViewHeight;
    shadowMapCamera.m_ViewWidth = desc.m_ViewWidth;

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, shadowMapCamera);
    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);
    graphics.SetRenderTarget(shadowMapTexture);

    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        for (Mesh* mesh : sceneObj->m_MeshRenderer.m_Meshes)
        {
            mesh->Draw(deviceContext);
        }
    }

    graphics.Clear(DirectX::Colors::Transparent, 1.0f, 0);
    graphics.UnbindRenderTargetView();
}
