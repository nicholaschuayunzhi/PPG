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
#include "../Transform.h"

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

        lightCubeMesh = new Mesh(std::move(vertices), std::move(indices), graphics);
        planeMesh = new Mesh(std::move(planeVertices), std::move(planeIndices), graphics);
        shader = new Shader(L"VertexShader.cso", L"PixelShader.cso", graphics);
        brickTexture = new Texture(L"..\\..\\Data\\Brick_Wall_014_COLOR.jpg", graphics);
        brickNormalMap = new Texture(L"..\\..\\Data\\Brick_Wall_014_NORM.jpg", graphics);
        sampler = new Sampler(graphics);

        camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
        modelBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());

        planeMaterial
            .SetAmbient(0.1, 0.1, 0.1)
            .SetSpecular(0.5, 0.5, 0.5)
            .SetShininess(32)
            .UseDiffuseMap(brickTexture)
            .UseNormalMap(brickNormalMap)
            .Update(graphics);

        lightMaterial
            .SetEmissive(lightColour.x, lightColour.y, lightColour.z)
            .Update(graphics);

        Light pointLight;
        pointLight.m_Color = lightColour;
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        lightManager
            .AddLight(pointLight)
            .SetGlobalAmbient(XMFLOAT4(0.2, 0.2, 0.2, 0))
            .SetEyePosition(camera.m_EyePosition)
            .Update(graphics);

        stormTfm
            .SetPosition(0, -1, 0)
            .UniformScale(0.8f);

        planeTfm
            .SetPosition(0, -1, 0)
            .UniformScale(5);

        lightCubeTfm
            .SetPosition(4, 3, 0)
            .UniformScale(0.5);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        camera.HandleMovement(input, deltaTime);
        camera.UpdateView(graphics, camera.CalculateView());
        lightManager.SetEyePosition(camera.m_EyePosition);

        static float phase = 0;
        phase += 90 * deltaTime;
        float zDisplacement = 3 * sin(XMConvertToRadians(phase));

        lightCubeTfm
            .SetPosition(4, 3, zDisplacement);

        Light& pointLight = lightManager.GetLight(0);
        XMStoreFloat4(&(pointLight.m_Position), lightCubeTfm.position);
        lightManager.Update(graphics);

        auto deviceContext = graphics.m_DeviceContext;
        camera.Use(deviceContext);
        shader->Use(deviceContext);
        lightManager.Use(deviceContext, 1); // should be linked to material + shader
        sampler->Use(deviceContext, 0);
        deviceContext->VSSetConstantBuffers(0, 1, &modelBuffer);

        lightCubeTfm.Update(graphics, modelBuffer);
        lightCubeMesh->Draw(deviceContext, &lightMaterial);

        planeTfm.Update(graphics, modelBuffer);
        planeMesh->Draw(deviceContext, &planeMaterial);

        stormTfm.Update(graphics, modelBuffer);
        stormtrooper->Draw(deviceContext);
    }

    void End()
    {
        if (planeMesh) delete planeMesh;
        if (lightCubeMesh) delete lightCubeMesh;
        if (shader) delete shader;
        if (brickTexture) delete brickTexture;
        if (brickNormalMap) delete brickNormalMap;
        if (sampler) delete sampler;
        if (stormtrooper) delete stormtrooper;

        SafeRelease(modelBuffer);
    }

private:
    Shader* shader;
    Texture* brickTexture;
    Texture* brickNormalMap;
    Sampler* sampler;

    Camera camera;

    Transform planeTfm;
    Material planeMaterial;
    Mesh* planeMesh;

    Transform lightCubeTfm;
    Mesh* lightCubeMesh;
    Material lightMaterial;

    Transform stormTfm;
    Model* stormtrooper;

    LightManager lightManager;
    XMFLOAT4 lightColour = XMFLOAT4(Colors::GhostWhite);

    ID3D11Buffer* modelBuffer;
};
