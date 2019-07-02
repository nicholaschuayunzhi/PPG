#pragma once
#include "../stdafx.h"
#include <math.h>

#include "Demo.h"
#include "../Camera.h"
#include "../Graphics.h"
#include "../Mesh.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../Sampler.h"
#include "../Light.h"
#include "../Material.h"

#include "../Model.h"

class NormalMapDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        stormtrooper = new Model("..\\..\\Data\\Models\\stormtrooper\\stormtrooper.obj", graphics);
        std::vector<Vertex> vertices;
        vertices.reserve(24);

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

        std::vector<WORD> indices =
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
        planeVertices.push_back({ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });
        planeVertices.push_back({ XMFLOAT3(1.0f,  0.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
        planeVertices.push_back({ XMFLOAT3(1.0f,  0.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
        std::vector<WORD> planeIndices = { 0, 1, 2, 0, 2, 3 };

        cubeMesh = new Mesh(std::move(vertices), std::move(indices), graphics);
        planeMesh = new Mesh(std::move(planeVertices), std::move(planeIndices), graphics);
        shader = new Shader(L"VertexShader.cso", L"PixelShader.cso", graphics);
        texture = new Texture(L"..\\..\\Data\\Brick_Wall_014_COLOR.jpg", graphics);
        normalMap = new Texture(L"..\\..\\Data\\Brick_Wall_014_NORM.jpg", graphics);
        sampler = new Sampler(graphics);

        XMMATRIX projection = camera.CalculateProjection(graphics.m_ClientRect);
        mvp[0] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[1] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
        mvp[2] = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &projection);

        material.m_Emissive = XMFLOAT4(0, 0, 0, 0);
        material.m_Ambient = XMFLOAT4(0.5, 0.5f, 0.5f, 0);
        material.m_Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0);
        material.m_Specular = XMFLOAT4(0.8, 0.8, 0.8, 0);
        material.m_Shininess = 32;
        material.m_UseTexture = 1;
        material.m_UseNormalMap = 1;
        materialBuffer = graphics.CreateBuffer(sizeof(Material), D3D11_BIND_CONSTANT_BUFFER, &material);

        lightMaterial.m_Emissive = lightColour;
        lightMaterial.m_Ambient = lightColour;
        lightMaterial.m_Diffuse = lightColour;
        lightMaterial.m_Specular = lightColour;
        lightMaterial.m_Shininess = 0;
        lightMaterial.m_UseTexture = 0;
        lightMaterialBuffer = graphics.CreateBuffer(sizeof(Material), D3D11_BIND_CONSTANT_BUFFER, &lightMaterial);

        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);
        lp.m_GlobalAmbient = XMFLOAT4(0.2, 0.2, 0.2, 0);
        lp.m_Lights[0].m_Color = lightColour;
        lp.m_Lights[0].m_Position = XMFLOAT4(4, 3, 0, 0);
        lp.m_Lights[0].m_LightType = LightType::PointLight;
        lp.m_Lights[0].m_Enabled = 1;
        lightsBuffer = graphics.CreateBuffer(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER, &lp);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        camera.HandleMovement(input, deltaTime);

        XMMATRIX view = camera.CalculateView();
        graphics.UpdateBuffer(mvp[1], &view);
        XMStoreFloat4(&(lp.m_EyePosition), camera.m_EyePosition);

        // Cube
        auto deviceContext = graphics.m_DeviceContext;
        shader->Use(deviceContext);
        deviceContext->VSSetConstantBuffers(0, 3, mvp);
        deviceContext->PSSetConstantBuffers(1, 1, &lightsBuffer);
        sampler->Use(deviceContext, 0);

        // Point Light Cube
        static float phase = -45.0f;
        phase += 90.0f * deltaTime;
        float zDisplacement = 3 * sin(XMConvertToRadians(phase));
        XMMATRIX model = XMMatrixMultiply(XMMatrixScaling(0.5, 0.5, 0.5), XMMatrixTranslation(4, 3, zDisplacement));
        lp.m_Lights[0].m_Position = XMFLOAT4(4, 3, zDisplacement, 0);
        graphics.UpdateBuffer(lightsBuffer, &lp);
        graphics.UpdateBuffer(mvp[0], &model);
        deviceContext->PSSetConstantBuffers(0, 1, &lightMaterialBuffer);
        cubeMesh->Draw(deviceContext);

        XMVECTOR rotationAxis = XMVectorSet(0, 1, 0.3, 0);
        model = XMMatrixMultiply(XMMatrixScaling(5, 5, 5), XMMatrixTranslation(0, -1, 0));
        graphics.UpdateBuffer(mvp[0], &model);
        deviceContext->PSSetConstantBuffers(0, 1, &materialBuffer);
        texture->Use(deviceContext, 0);
        normalMap->Use(deviceContext, 1);
        planeMesh->Draw(deviceContext);

        model = XMMatrixTranslation(0, -1, 0);
        graphics.UpdateBuffer(mvp[0], &model);
        deviceContext->PSSetConstantBuffers(0, 1, &materialBuffer);
        stormtrooper->Draw(deviceContext);
    }

    void End()
    {
        if (planeMesh) delete planeMesh;
        if (cubeMesh) delete cubeMesh;
        if (shader) delete shader;
        if (texture) delete texture;
        if (normalMap) delete normalMap;
        if (sampler) delete sampler;
        if (stormtrooper) delete stormtrooper;

        SafeRelease(mvp[0]);
        SafeRelease(mvp[1]);
        SafeRelease(mvp[2]);
        SafeRelease(lightsBuffer);
    }

private:
    Model* stormtrooper;
    Camera camera;
    LightProperties lp;
    Material material;
    Material lightMaterial;
    XMFLOAT4 lightColour = XMFLOAT4(Colors::GhostWhite);
    Mesh* planeMesh;
    Mesh* cubeMesh;
    Shader* shader;
    Texture* texture;
    Texture* normalMap;
    Sampler* sampler;
    ID3D11Buffer* lightsBuffer;
    ID3D11Buffer* materialBuffer;
    ID3D11Buffer* lightMaterialBuffer;
    ID3D11Buffer* mvp[3];
};
