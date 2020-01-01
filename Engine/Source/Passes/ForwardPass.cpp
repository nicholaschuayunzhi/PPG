#include "stdafx.h"
#include "ForwardPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material/PBRMaterial.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ForwardPass::ForwardPass(Graphics& graphics, Texture& renderTarget) :
    m_RenderTarget(renderTarget)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"Forward.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(PBRMaterialInfo), D3D11_BIND_CONSTANT_BUFFER, nullptr);
}

ForwardPass::~ForwardPass()
{
    SafeRelease(m_Buffer);
}

void ForwardPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.ClearRenderTargetView(m_RenderTarget.m_TextureRTV, Colors::Transparent);
    graphics.SetRenderTarget(m_RenderTarget);

    auto deviceContext = graphics.m_DeviceContext;
    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.lightManager.Use(deviceContext, 1); // should be linked to material + shader
    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);
    auto& lightManager = scene.lightManager;
    if (lightManager.hasLightWithShadows)
    {
        deviceContext->PSSetConstantBuffers(2, 1, &(lightManager.m_OneShadowMapCBuffer));
        lightManager.m_OneShadowMapTexture->UseSRV(deviceContext, 4);
    }

    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        PBRMaterial* mat = sceneObj->m_MeshRenderer.m_Material;
        graphics.UpdateBuffer(m_Buffer, &(mat->m_MaterialInfo));
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        if (mat->m_Albedo)
            mat->m_Albedo->UseSRV(deviceContext, 0);
        if (mat->m_Normal)
            mat->m_Normal->UseSRV(deviceContext, 1);
        deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);
        sceneObj->m_MeshRenderer.m_Mesh->Draw(deviceContext);
    }

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(4);
    graphics.UnbindRenderTargetView();
}
