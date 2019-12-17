#include "stdafx.h"
#include "ForwardPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Material.h"
#include "Scene/Light.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

ForwardPass::ForwardPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"PixelShader.cso", graphics);
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

    for (auto& sceneObj : scene.m_Objects)
    {
        if (sceneObj.m_Mesh == nullptr) continue;
        scene.UpdateModel(graphics, sceneObj.m_Transform.GetModel());
        sceneObj.m_Material->Use(deviceContext);
        sceneObj.m_Mesh->Draw(deviceContext);
    }

    Texture::SetNullSrv(deviceContext, 3);
}
