#include "stdafx.h"
#include "GBufferPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material/PhongMaterial.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

GBufferPass::GBufferPass(Graphics& graphics, Texture& diffuse, Texture& specular, Texture& normals) :
    m_Diffuse(diffuse),
    m_Specular(specular),
    m_Normals(normals)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"GBuffer.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(MaterialInfo), D3D11_BIND_CONSTANT_BUFFER, nullptr);

    m_RenderTargets[0] = m_Diffuse.m_TextureRTV;
    m_RenderTargets[1] = m_Specular.m_TextureRTV;
    m_RenderTargets[2] = m_Normals.m_TextureRTV;
}

GBufferPass::~GBufferPass()
{
    SafeRelease(m_Buffer);
}

void GBufferPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    graphics.ClearRenderTargetView(m_Diffuse.m_TextureRTV, Colors::Transparent);
    graphics.ClearRenderTargetView(m_Specular.m_TextureRTV, Colors::Transparent);
    graphics.ClearRenderTargetView(m_Normals.m_TextureRTV, Colors::Transparent);

    deviceContext->OMSetRenderTargets(3, &(m_RenderTargets[0]), graphics.m_DepthStencilBuffer->m_TextureDSV);

    scene.UseCamera(graphics, scene.m_MainCamera);
    scene.lightManager.Use(deviceContext, 1);
    scene.UseModel(graphics);
    shader->Use(deviceContext);

    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        PhongMaterial* phongMat = sceneObj->m_MeshRenderer.m_Material;
        graphics.UpdateBuffer(m_Buffer, &(phongMat->m_MaterialInfo));
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        if (phongMat->m_Diffuse)
            phongMat->m_Diffuse->UseSRV(deviceContext, 0);
        if (phongMat->m_Normal)
            phongMat->m_Normal->UseSRV(deviceContext, 1);
        if (phongMat->m_Specular)
            phongMat->m_Specular->UseSRV(deviceContext, 2);
        deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);
        sceneObj->m_MeshRenderer.m_Mesh->Draw(deviceContext);
    }

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
    graphics.UnbindRenderTargetView();
}
