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

class CubeDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        std::vector<Vertex> vertices;
        vertices.reserve(24);

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3( 0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT3( 0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT3( 0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3( 0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT3( 0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3( 0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3( 0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT3( 0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT3( 1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT3( 1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT3( 1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT3( 1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3( 0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3( 0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT3( 0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT3( 0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        std::vector<WORD> indices =
        {
            0,  1,  2,  0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9, 10,  8, 10, 11,
           12, 13, 14, 12, 14, 15,
           16, 17, 18, 16, 18, 19,
           20, 21, 22, 20, 22, 23
        };

        cubeMesh = new Mesh(std::move(vertices), std::move(indices), graphics);
        shader = new Shader(L"VertexShader.cso", L"PixelShader.cso", graphics);
        texture = new Texture(L"..\\..\\Data\\Container.jpg", graphics);
        sampler = new Sampler(graphics);

        XMMATRIX projection = camera.CalculateProjection(graphics.m_ClientRect);
        mvp[0] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[1] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[2] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &projection);

        material.m_Emissive = XMFLOAT4(0, 0, 0, 0);
        material.m_Ambient = XMFLOAT4(0.1, 0.2f, 0.2f, 0);
        material.m_Diffuse = XMFLOAT4(0.5f, 0.5f, 0.2f, 0);
        material.m_Specular = XMFLOAT4(1, 1, 1, 0);
        material.m_Shininess = 32;
        material.m_UseTexture = 0;
        materialBuffer = graphics.CreateBuffer(sizeof(Material), D3D11_BIND_CONSTANT_BUFFER, &material);

        lightMaterial.m_Emissive = XMFLOAT4(Colors::Red);
        lightMaterial.m_Ambient = XMFLOAT4(Colors::Red);
        lightMaterial.m_Diffuse = XMFLOAT4(Colors::Red);
        lightMaterial.m_Specular = XMFLOAT4(Colors::Red);
        lightMaterial.m_Shininess = 0;
        lightMaterial.m_UseTexture = 0;
        lightMaterialBuffer = graphics.CreateBuffer(sizeof(Material), D3D11_BIND_CONSTANT_BUFFER, &lightMaterial);

        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);
        lp.m_GlobalAmbient = XMFLOAT4(0.2, 0.2, 0.4, 0);

        lp.m_Lights[0].m_Color = XMFLOAT4(Colors::CornflowerBlue);
        lp.m_Lights[0].m_Direction = XMFLOAT4(1, -1, 1, 0);
        lp.m_Lights[0].m_LightType = LightType::DirectionalLight;
        lp.m_Lights[0].m_Enabled = 1;

        lp.m_Lights[1].m_Color = XMFLOAT4(Colors::Red);
        lp.m_Lights[1].m_Position = XMFLOAT4(0, 5, 5, 1);
        lp.m_Lights[1].m_LightType = LightType::PointLight;
        lp.m_Lights[1].m_Enabled = 1;

        lp.m_Lights[2].m_Color = XMFLOAT4(Colors::White);
        XMStoreFloat4(&(lp.m_Lights[2].m_Position), camera.m_EyePosition);
        XMStoreFloat4(&(lp.m_Lights[2].m_Direction), camera.m_Forward);
        lp.m_Lights[2].m_LightType = LightType::SpotLight;
        lp.m_Lights[2].m_SpotAngle = XMConvertToRadians(10.0f);
        lp.m_Lights[2].m_Enabled = 1;
        lightsBuffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &lp);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        camera.HandleMovement(input, deltaTime);

        XMMATRIX view = camera.CalculateView();
        graphics.UpdateBuffer(mvp[1], &view);

        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);
        XMStoreFloat4(&(lp.m_Lights[2].m_Position), camera.m_EyePosition); // spot light is a flash light
        XMStoreFloat4(&(lp.m_Lights[2].m_Direction), camera.m_Forward);
        graphics.UpdateBuffer(lightsBuffer, &lp);

        // Cube
        static float angle = 45.0f;
        //angle += 90.0f * deltaTime;
        XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
        XMMATRIX model = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        graphics.UpdateBuffer(mvp[0], &model);
        auto deviceContext = graphics.m_DeviceContext;

        shader->Use(deviceContext);
        deviceContext->VSSetConstantBuffers(0, 3, mvp);
        deviceContext->PSSetConstantBuffers(0, 1, &materialBuffer);
        deviceContext->PSSetConstantBuffers(1, 1, &lightsBuffer);
        texture->Use(deviceContext, 0); // not in use
        sampler->Use(deviceContext, 0);
        cubeMesh->Draw(deviceContext);

        // Point Light Cube
        model = XMMatrixMultiply(XMMatrixScaling(0.5, 0.5, 0.5), XMMatrixTranslation(0, 5, 5));

        graphics.UpdateBuffer(mvp[0], &model);
        shader->Use(deviceContext);
        deviceContext->VSSetConstantBuffers(0, 3, mvp);
        deviceContext->PSSetConstantBuffers(0, 1, &lightMaterialBuffer);
        deviceContext->PSSetConstantBuffers(1, 1, &lightsBuffer);
        sampler->Use(deviceContext, 0);
        cubeMesh->Draw(deviceContext);
    }

    void End()
    {
        if (cubeMesh) delete cubeMesh;
        if (shader) delete shader;
        if (texture) delete texture;
        if (sampler) delete sampler;
        SafeRelease(mvp[0]);
        SafeRelease(mvp[1]);
        SafeRelease(mvp[2]);
        SafeRelease(lightsBuffer);
    }

private:
    Camera camera;
    LightProperties lp;
    Material material;
    Material lightMaterial;
    Mesh* cubeMesh;
    Shader* shader;
    Texture* texture;
    Sampler* sampler;
    ID3D11Buffer* lightsBuffer;
    ID3D11Buffer* materialBuffer;
    ID3D11Buffer* lightMaterialBuffer;
    ID3D11Buffer* mvp[3];
};
