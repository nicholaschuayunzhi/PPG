#pragma once
#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        lightCubeMesh = new Mesh(CubeVertices(), CubeIndices(), graphics);
        planeMesh = new Mesh(QuadVertices(), QuadIndices(), graphics);
        brickTexture = new Texture(L"Data\\Brick_Wall_014_COLOR.jpg", graphics);
        brickNormalMap = new Texture(L"Data\\Brick_Wall_014_NORM.jpg", graphics);
        sampler = new Sampler(graphics);

        forwardPass = std::make_unique<ForwardPass>(graphics);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, L"Data\\skybox.dds");

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

        // Create scene objects before referencing..
        lightCube = scene.CreateSceneObject("LightCube", lightCubeMesh, &lightMaterial);
        plane = scene.CreateSceneObject("Plane", planeMesh, &planeMaterial);
        stormtrooper = Model::LoadModelToScene("Data\\Models\\stormtrooper\\stormtrooper.obj", scene, graphics);

        SceneObject& stormTrooperObj = scene.GetSceneObjectByIndex(stormtrooper->m_RootIndex);
        SceneObject& lightCubeObj = scene.GetSceneObjectByIndex(lightCube);
        SceneObject& planeObj = scene.GetSceneObjectByIndex(plane);
        lightCubeObj.m_Transform
            .UniformScale(0.5);

        planeObj.m_Transform
            .SetPosition(0, -1, 0)
            .UniformScale(5);

        stormTrooperObj.m_Transform
            .SetPosition(0, -1, 0)
            .RotateEulerAngles(0.3, 0, 0);

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
        auto& lightCubeObj = scene.GetSceneObjectByIndex(lightCube);
        lightCubeObj.m_Transform.SetPosition(-2, 3, zDisplacement);

        Light& pointLight = scene.lightManager.GetLight(0);
        XMStoreFloat4(&(pointLight.m_Position), lightCubeObj.m_Transform.position);

        auto deviceContext = graphics.m_DeviceContext;
        sampler->Use(deviceContext, 0);

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

    Scene::ObjectIndex plane;
    Scene::ObjectIndex lightCube;

    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SkyboxPass> skyboxPass;
};
