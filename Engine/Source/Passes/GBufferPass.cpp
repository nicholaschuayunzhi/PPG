#include "stdafx.h"
#include "GBufferPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material/PBRMaterial.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Scene/Model/Skeleton.h"

GBufferPass::GBufferPass(Graphics& graphics, Texture& diffuse, Texture& metalRough, Texture& normals) :
    m_Diffuse(diffuse),
    m_MetalRough(metalRough),
    m_Normals(normals)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"GBuffer.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(PBRMaterialInfo), D3D11_BIND_CONSTANT_BUFFER, nullptr);
    m_BoneBuffer = graphics.CreateBuffer(sizeof(XMMATRIX) * Skeleton::NUM_BONES, D3D11_BIND_CONSTANT_BUFFER, nullptr);
    m_RenderTargets[0] = m_Diffuse.GetRTV();
    m_RenderTargets[1] = m_MetalRough.GetRTV();
    m_RenderTargets[2] = m_Normals.GetRTV();
}

GBufferPass::~GBufferPass()
{
    SafeRelease(m_Buffer);
    SafeRelease(m_BoneBuffer);
}

void GBufferPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    graphics.ClearRenderTargetView(m_Diffuse.GetRTV(), Colors::Transparent);
    graphics.ClearRenderTargetView(m_MetalRough.GetRTV(), Colors::Transparent);
    graphics.ClearRenderTargetView(m_Normals.GetRTV(), Colors::Transparent);

    deviceContext->OMSetRenderTargets(3, &(m_RenderTargets[0]), graphics.m_DepthStencilBuffer->m_TextureDSV);

    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.lightManager.Use(deviceContext, 1);
    scene.UseModel(graphics);
    shader->Use(deviceContext);
    deviceContext->VSSetConstantBuffers(3, 1, &m_BoneBuffer);
    deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);

    Animator* currentAnimator = nullptr;

    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        MeshRenderer& meshRenderer = sceneObj->m_MeshRenderer;
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        Animator* animator = meshRenderer.m_Animator;
        if (animator != nullptr && animator->m_IsEnabled)
        {
            if (animator != currentAnimator)
            {
                graphics.UpdateBuffer(m_BoneBuffer, animator->m_FinalTransforms);
                currentAnimator = animator;
            }
        }
        PBRMaterial* mat = meshRenderer.m_Material;
        graphics.UpdateBuffer(m_Buffer, &(mat->m_MaterialInfo));
        if (mat->m_Albedo)
            mat->m_Albedo->UseSRV(deviceContext, 0);
        if (mat->m_Normal)
            mat->m_Normal->UseSRV(deviceContext, 1);
        if (mat->m_OccRoughMetal)
            mat->m_OccRoughMetal->UseSRV(deviceContext, 2);
        if (mat->m_AoMap)
            mat->m_AoMap->UseSRV(deviceContext, 3);
        meshRenderer.m_Mesh->Draw(deviceContext);
    }

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
    graphics.UnbindShaderResourceView(3);
    graphics.UnbindRenderTargetView();
}
