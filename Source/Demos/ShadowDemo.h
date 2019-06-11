#pragma once
#include "../stdafx.h"
#include "Demo.h"
#include "../Camera.h"
#include "../Graphics.h"
#include "../Mesh.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../Sampler.h"
#include "../Light.h"
#include "../Material.h"

class ShadowDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        std::vector<Vertex> cubeVertices;
        cubeVertices.reserve(24);

        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) });

        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) });

        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        cubeVertices.push_back({ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        cubeVertices.push_back({ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        std::vector<WORD> cubeIndices =
        {
            0,  1,  2,  0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9, 10,  8, 10, 11,
           12, 13, 14, 12, 14, 15,
           16, 17, 18, 16, 18, 19,
           20, 21, 22, 20, 22, 23
        };

        std::vector<Vertex> planeVertices;
        planeVertices.reserve(4);
        planeVertices.push_back({ XMFLOAT3(-1.0f,  0.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        planeVertices.push_back({ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        planeVertices.push_back({ XMFLOAT3( 1.0f,  0.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        planeVertices.push_back({ XMFLOAT3( 1.0f,  0.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });
        std::vector<WORD> planeIndices = { 0, 1, 2, 0, 2, 3 };

        cubeMesh = new Mesh(std::move(cubeVertices), std::move(cubeIndices), graphics);
        planeMesh = new Mesh(std::move(planeVertices), std::move(planeIndices), graphics);
        shader = new Shader(L"Shadow.vs.cso", L"Shadow.ps.cso", graphics);
        shadowMapShader = new Shader(L"VertexShader.cso", L"ShadowMap.ps.cso", graphics);
        RECT clientRect = graphics.m_ClientRect;
        shadowMapTexture = new Texture(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, graphics);
        texture = new Texture(L"..\\..\\Data\\Container.jpg", graphics);
        sampler = new Sampler(graphics);

        material.m_Emissive = XMFLOAT4(0, 0, 0, 0);
        material.m_Ambient = XMFLOAT4(0.1, 0.2f, 0.2f, 0);
        material.m_Diffuse = XMFLOAT4(0.8f, 0.5f, 0.2f, 0);
        material.m_Specular = XMFLOAT4(1, 1, 1, 0);
        material.m_Shininess = 32;
        material.m_UseTexture = 1;
        materialBuffer = graphics.CreateBuffer(sizeof(Material), D3D11_BIND_CONSTANT_BUFFER, &material);

        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);
        lp.m_GlobalAmbient = XMFLOAT4(0.2, 0.2, 0.4, 0);
        lp.m_Lights[0].m_Color = XMFLOAT4(Colors::White);
        lp.m_Lights[0].m_Direction = XMFLOAT4(-1, -1, 0, 0);
        lp.m_Lights[0].m_LightType = LightType::DirectionalLight;
        lp.m_Lights[0].m_Enabled = 1;
        lightsBuffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &lp);

        XMMATRIX projection = camera.CalculateProjection(graphics.m_ClientRect);
        mvp[0] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[1] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[2] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());

        RenderShadowMap(graphics);

        graphics.UpdateBuffer(mvp[2], &projection);
    }

    void RenderShadowMap(Graphics& graphics)
    {
        XMVECTOR position = XMLoadFloat4(&(lp.m_Lights[0].m_Direction));
        position *= -5;
        XMVECTOR focus = XMVectorSet(0, 0, 0, 1);
        XMMATRIX view = XMMatrixLookAtLH(position, focus, XMVectorSet(0, 1, 0, 1));
        XMMATRIX projection = XMMatrixOrthographicLH(16, 12, 4.0f, 20.0f);
        graphics.UpdateBuffer(mvp[1], &view);
        graphics.UpdateBuffer(mvp[2], &projection);

        lightViewProjection = XMMatrixMultiply(view, projection);
        lightVPBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &lightViewProjection);

        // Render
        graphics.Clear(Colors::White, 1.0f, 0);
        auto deviceContext = graphics.m_DeviceContext;
        deviceContext->OMSetRenderTargets(1, &shadowMapTexture->m_TextureRTV, graphics.m_DepthStencilView);
        shadowMapShader->Use(deviceContext);
        deviceContext->VSSetConstantBuffers(0, 3, mvp);

        // Cube
        static float angle = 45.0f;
        XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
        XMMATRIX model = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        graphics.UpdateBuffer(mvp[0], &model);
        cubeMesh->Draw(deviceContext);

        // Plane
        model = XMMatrixMultiply(XMMatrixScaling(5, 5, 5), XMMatrixTranslation(0, -1, 0));
        graphics.UpdateBuffer(mvp[0], &model);
        planeMesh->Draw(deviceContext);

        // Reset State
        graphics.SetUp();
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        camera.HandleMovement(input, deltaTime);

        XMMATRIX view = camera.CalculateView();
        graphics.UpdateBuffer(mvp[1], &view);

        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);
        graphics.UpdateBuffer(lightsBuffer, &lp);

        auto deviceContext = graphics.m_DeviceContext;
        shader->Use(deviceContext);
        texture->Use(deviceContext, 0);
        shadowMapTexture->Use(deviceContext, 1);
        sampler->Use(deviceContext, 0);
        deviceContext->VSSetConstantBuffers(0, 3, mvp);
        deviceContext->VSSetConstantBuffers(3, 1, &lightVPBuffer);
        deviceContext->PSSetConstantBuffers(0, 1, &materialBuffer);
        deviceContext->PSSetConstantBuffers(1, 1, &lightsBuffer);

        // Cube
        static float angle = 45.0f;
        XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
        XMMATRIX model = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        graphics.UpdateBuffer(mvp[0], &model);
        cubeMesh->Draw(deviceContext);

        // Plane
        model = XMMatrixMultiply(XMMatrixScaling(5, 5, 5), XMMatrixTranslation(0, -1, 0));
        graphics.UpdateBuffer(mvp[0], &model);
        planeMesh->Draw(deviceContext);
    }

    void End()
    {
        if (cubeMesh) delete cubeMesh;
        if (planeMesh) delete planeMesh;

        if (shader) delete shader;
        if (texture) delete texture;
        if (sampler) delete sampler;

        SafeRelease(mvp[0]);
        SafeRelease(mvp[1]);
        SafeRelease(mvp[2]);
        SafeRelease(lightsBuffer);

        if (shadowMapShader) delete shadowMapShader;
        if (shadowMapTexture) delete shadowMapTexture;
        SafeRelease(lightVPBuffer);
    }

private:
    Camera camera;
    LightProperties lp;
    Material material;

    Mesh* cubeMesh;
    Mesh* planeMesh;
    Shader* shader;
    Texture* texture;
    Sampler* sampler;
    ID3D11Buffer* lightsBuffer;
    ID3D11Buffer* materialBuffer;
    ID3D11Buffer* mvp[3];

    // Shadow Map
    Shader* shadowMapShader;
    Texture* shadowMapTexture;
    XMMATRIX lightViewProjection;
    ID3D11Buffer* lightVPBuffer;
};
