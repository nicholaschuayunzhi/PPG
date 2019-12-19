#pragma once
#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        // Rendering Stuff
        auto& clientRect = graphics.m_ClientRect;
        unsigned int clientWidth = clientRect .right - clientRect.left;
        unsigned int clientHeight = clientRect.bottom - clientRect.top;
        colour = std::make_unique<Texture>(clientWidth, clientHeight, graphics, "Colour");
        sampler = new Sampler(graphics);
        auto& colourTexture = *(colour.get());
        forwardPass = std::make_unique<ForwardPass>(graphics, colourTexture);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\sky.dds");
        blitPass = std::make_unique<BlitPass>(graphics, colourTexture, *(graphics.m_BackBuffer.get()));

        // Lighting
        auto lightColour = XMFLOAT4(Colors::White);
        Light pointLight;
        pointLight.m_Color = XMFLOAT4(DirectX::Colors::MediumPurple);
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        Light dirLight;
        dirLight.m_Color = lightColour;
        dirLight.m_Direction = XMFLOAT4(-1, -1, 1, 0);
        dirLight.m_LightType = LightType::DirectionalLight;

        scene.lightManager
            .AddLight(pointLight)
            .AddLight(dirLight)
            .SetGlobalAmbient(XMFLOAT4(0, 0, 0, 0));

        ShadowMapRenderDesc desc;
        desc.m_EyePosition = XMLoadFloat4(&(scene.lightManager.GetLight(1).m_Direction)) * -5;
        desc.m_LookAt = XMVectorSet(0, 0, 0, 1);
        desc.m_ViewWidth = 16;
        desc.m_ViewHeight = 12;
        desc.m_NearZ = 1;
        desc.m_FarZ = 20;
        desc.m_TextureWidth = clientRect.right - clientRect.left;
        desc.m_TextureHeight = clientRect.bottom - clientRect.top;
        scene.lightManager.SetLightWithShadows(graphics, 1, desc);

        // Objects
        lightCube = scene.CreateSceneObject("LightCube");
        lightCube->m_Transform
            .UniformScale(0.5);
        lightCubeMesh = new Mesh(CubeVertices(), CubeIndices(), graphics);
        lightMaterial
            .SetEmissive(pointLight.m_Color.x, pointLight.m_Color.y, pointLight.m_Color.z);
        auto& lightCubeMeshRenderer = lightCube->m_MeshRenderer;
        lightCubeMeshRenderer.m_Mesh = lightCubeMesh;
        lightCubeMeshRenderer.m_Material = &lightMaterial;
        lightCubeMeshRenderer.m_IsEnabled = true;

        plane = scene.CreateSceneObject("Plane");
        plane->m_Transform
            .SetPosition(0, -1, 0)
            .UniformScale(5);
        planeMesh = new Mesh(QuadVertices(), QuadIndices(), graphics);
        brickTexture = new Texture(L"Data\\Brick_Wall_014_COLOR.jpg", graphics);
        brickNormalMap = new Texture(L"Data\\Brick_Wall_014_NORM.jpg", graphics);
        planeMaterial
            .SetAmbient(0.1, 0.1, 0.1)
            .SetSpecular(0.5, 0.5, 0.5)
            .SetShininess(32)
            .UseDiffuseMap(brickTexture)
            .UseNormalMap(brickNormalMap);
        auto& planeMeshRenderer = plane->m_MeshRenderer;
        planeMeshRenderer.m_Mesh = planeMesh;
        planeMeshRenderer.m_Material = &planeMaterial;
        planeMeshRenderer.m_IsEnabled = true;

        stormtrooper = Model::LoadModelToScene("Data\\Models\\stormtrooper\\stormtrooper.obj", scene, graphics);
        auto stormTrooperObj = scene.GetSceneObjectByIndex(stormtrooper->m_RootIndex);
        stormTrooperObj->m_Transform
            .SetPosition(0, -1, 0)
            .RotateEulerAngles(0.3, 0, 0);


        scene.Start(graphics);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        static float phase = 0;
        phase += 90 * deltaTime;
        float zDisplacement = 3 * sin(XMConvertToRadians(phase));
        lightCube->m_Transform.SetPosition(-2, 3, zDisplacement);

        Light& pointLight = scene.lightManager.GetLight(0);
        XMStoreFloat4(&(pointLight.m_Position), lightCube->m_Transform.position);

        auto deviceContext = graphics.m_DeviceContext;
        sampler->Use(deviceContext, 0);

        scene.Update(graphics, input, deltaTime);
        forwardPass->Render(graphics, scene);
        skyboxPass->Render(graphics, scene);
        blitPass->Render(graphics, scene);
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

    PhongMaterial planeMaterial;
    Mesh* planeMesh;

    Mesh* lightCubeMesh;
    PhongMaterial lightMaterial;

    Model* stormtrooper;

    std::shared_ptr<SceneObject> plane;
    std::shared_ptr<SceneObject> lightCube;

    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SkyboxPass> skyboxPass;
    std::unique_ptr<BlitPass> blitPass;
    std::unique_ptr<Texture> colour;
};
