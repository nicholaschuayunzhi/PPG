#include "stdafx.h"
#include "DeferredPass.h"
#include "LowLevel/Graphics.h"
#include "Scene/Scene.h"
#include "Scene/Light.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"

struct DeferredPassCBuffer
{
    XMMATRIX m_InverseProjection;
    XMMATRIX m_InverseView;
};

DeferredPass::DeferredPass(Graphics& graphics, Texture& renderTarget, Texture& diffuse, Texture& specular, Texture& normals) :
    m_Diffuse(diffuse),
    m_Specular(specular),
    m_Normals(normals),
    m_RenderTarget(renderTarget)
{
    m_Shader = std::make_unique<Shader>(L"Fullscreen.vs.cso", L"Deferred.ps.cso", graphics);
    m_Buffer = graphics.CreateBuffer(sizeof(DeferredPassCBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr);
}

DeferredPass::~DeferredPass()
{
}

void DeferredPass::Render(Graphics& graphics, Scene& scene)
{
    graphics.ClearRenderTargetView(m_RenderTarget.m_TextureRTV, Colors::Transparent);
    graphics.SetRenderTarget(m_RenderTarget, false);

    auto deviceContext = graphics.m_DeviceContext;
    scene.lightManager.Use(deviceContext, 1);
    auto& lightManager = scene.lightManager;
    if (lightManager.hasLightWithShadows)
    {
        deviceContext->PSSetConstantBuffers(2, 1, &(lightManager.m_OneShadowMapCBuffer));
        lightManager.m_OneShadowMapTexture->Use(deviceContext, 4);
    }

    auto& camera = scene.m_MainCamera;
    DeferredPassCBuffer deferredCBuf;
    deferredCBuf.m_InverseProjection = XMMatrixInverse(NULL, camera.CalculateProjection());
    deferredCBuf.m_InverseView = XMMatrixInverse(NULL, camera.CalculateView());
    deviceContext->PSSetConstantBuffers(3, 1, &m_Buffer);
    graphics.UpdateBuffer(m_Buffer, &(deferredCBuf));

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->PSSetShaderResources(0, 1, &(graphics.m_DepthSRV));
    m_Diffuse.Use(deviceContext, 1);
    m_Specular.Use(deviceContext, 2);
    m_Normals.Use(deviceContext, 3);
    m_Shader->Use(deviceContext);
    deviceContext->Draw(4, 0);

    graphics.UnbindShaderResourceView(0);
    graphics.UnbindShaderResourceView(1);
    graphics.UnbindShaderResourceView(2);
    graphics.UnbindShaderResourceView(4);
    graphics.UnbindRenderTargetView();
}
