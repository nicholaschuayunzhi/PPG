#pragma once
#include "../stdafx.h"
#include <math.h>

#include "Demo.h"
#include "Camera.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Sampler.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Transform.h"
#include "Scene.h"
#include "Passes/ForwardPass.h"
#include "Passes/SkyboxPass.h"

class NormalMapDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
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
        brickTexture = new Texture(L"..\\..\\Data\\Brick_Wall_014_COLOR.jpg", graphics);
        brickNormalMap = new Texture(L"..\\..\\Data\\Brick_Wall_014_NORM.jpg", graphics);
        sampler = new Sampler(graphics);

        forwardPass = std::make_unique<ForwardPass>(graphics);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, L"..\\..\\Data\\skybox.dds");

        planeMaterial
            .SetAmbient(0.1, 0.1, 0.1)
            .SetSpecular(0.5, 0.5, 0.5)
            .SetShininess(32)
            .UseDiffuseMap(brickTexture)
            .UseNormalMap(brickNormalMap)
            .Update(graphics);

        auto lightColour = XMFLOAT4(Colors::Orange);
        Light pointLight;
        pointLight.m_Color = XMFLOAT4(DirectX::Colors::MediumPurple);
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        lightMaterial
            .SetEmissive(pointLight.m_Color.x, pointLight.m_Color.y, pointLight.m_Color.z)
            .Update(graphics);

        Light dirLight;
        dirLight.m_Color = lightColour;
        dirLight.m_Direction = XMFLOAT4(1, -1, -1, 0);
        dirLight.m_LightType = LightType::DirectionalLight;

        scene.lightManager
            .AddLight(pointLight)
            .AddLight(dirLight)
            .SetGlobalAmbient(XMFLOAT4(0, 0, 0, 0));

        lightCube = scene.CreateSceneObject("LightCube", lightCubeMesh, &lightMaterial);
        plane = scene.CreateSceneObject("Plane", planeMesh, &planeMaterial);
        stormtrooper = ModelLoader::LoadModelToScene("..\\..\\Data\\Models\\stormtrooper\\stormtrooper.obj", scene, graphics);

        lightCube->m_Transform
            .UniformScale(0.5);

        plane->m_Transform
            .SetPosition(0, -1, 0)
            .UniformScale(5);

        stormtrooper->m_Root->m_Transform
            .SetPosition(0, -1, 0)
            .RotateEulerAngles(0.3, 0, 0)
            .UniformScale(1);

        RECT& clientRect = graphics.m_ClientRect;
        ShadowMapRenderDesc desc;
        desc.position = XMLoadFloat4(&(scene.lightManager.GetLight(1).m_Direction));
        desc.position *= -5;
        desc.focus = XMVectorSet(0, 0, 0, 1);
        desc.projection = XMMatrixOrthographicLH(16, 12, 1.0f, 20.0f);
        desc.view = desc.CalculateView();
        desc.textureWidth = clientRect.right - clientRect.left;
        desc.textureHeight = clientRect.bottom - clientRect.top;
        scene.lightManager.SetLightWithShadows(graphics, 1, desc);

        scene.Start(graphics);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        static float phase = 0;
        phase += 90 * deltaTime;
        float zDisplacement = 3 * sin(XMConvertToRadians(phase));
        lightCube->m_Transform.SetPosition(2, 3, zDisplacement);

        Light& pointLight = scene.lightManager.GetLight(0);
        XMStoreFloat4(&(pointLight.m_Position), lightCube->m_Transform.position);
        scene.lightManager.Update(graphics);

        auto deviceContext = graphics.m_DeviceContext;
        sampler->Use(deviceContext, 0);

        scene.lightManager.RenderAnyShadowMap(graphics, scene);
        scene.Update(graphics, input, deltaTime);
        forwardPass->Render(graphics, scene);
        skyboxPass->Render(graphics, scene);
    }

    void End()
    {
        if (planeMesh) delete planeMesh;
        if (lightCubeMesh) delete lightCubeMesh;
        if (brickTexture) delete brickTexture;
        if (brickNormalMap) delete brickNormalMap;
        if (sampler) delete sampler;
        if (stormtrooper) delete stormtrooper;
    }

private:
    Scene scene;

    Texture* brickTexture;
    Texture* brickNormalMap;

    Sampler* sampler;

    Material planeMaterial;
    Mesh* planeMesh;

    Mesh* lightCubeMesh;
    Material lightMaterial;

    Model* stormtrooper;

    SceneObject* plane;
    SceneObject* lightCube;

    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SkyboxPass> skyboxPass;
};
