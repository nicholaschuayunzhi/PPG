#include "stdafx.h"
#include "ForwardPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material/PhongMaterial.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ForwardPass::ForwardPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"PixelShader.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(MaterialInfo), D3D11_BIND_CONSTANT_BUFFER, nullptr);
}

ForwardPass::~ForwardPass()
{
    SafeRelease(m_Buffer);
}

void ForwardPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    scene.camera.Use(deviceContext);
    scene.lightManager.Use(deviceContext, 1); // should be linked to material + shader

    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);
    auto& lightManager = scene.lightManager;
    deviceContext->PSSetConstantBuffers(2, 1, &(lightManager.m_OneShadowMapCBuffer));
    lightManager.m_OneShadowMapTexture->Use(deviceContext, 3);

    for (auto sceneObj : scene.m_Objects)
    {
        if (!sceneObj->m_MeshRenderer.m_IsEnabled) continue;

        PhongMaterial* phongMat = sceneObj->m_MeshRenderer.m_Material;
        graphics.UpdateBuffer(m_Buffer, &(phongMat->m_MaterialInfo));
        scene.UpdateModel(graphics, sceneObj->m_Transform.GetModel());
        if (phongMat->m_Diffuse)
            phongMat->m_Diffuse->Use(deviceContext, 0);
        if (phongMat->m_Normal) 
            phongMat->m_Normal->Use(deviceContext, 1);
        if (phongMat->m_Specular)
            phongMat->m_Specular->Use(deviceContext, 2);
        deviceContext->PSSetConstantBuffers(0, 1, &m_Buffer);
        sceneObj->m_MeshRenderer.m_Mesh->Draw(deviceContext);
    }

    Texture::SetNullSrv(deviceContext, 3);
}
