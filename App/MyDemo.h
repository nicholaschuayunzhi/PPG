#pragma once
#include "../Engine/Source/PPG.h"
#include <math.h>

class MyDemo : public Demo
{
private:
    Scene scene;

    std::unique_ptr<Texture> brickTexture;
    std::unique_ptr<Texture> brickNormalMap;
    std::unique_ptr<Texture> lightBulbTex;
    std::unique_ptr<Texture> arrowTex;

    std::unique_ptr<Sampler> linearSampler;
    std::unique_ptr<Sampler> pointSampler;

    PhongMaterial planeMaterial;
    Mesh* planeMesh;

    Mesh* cubeMesh;
    PhongMaterial cubeMaterial;

    Model* stormtrooper;
    Model* sponza;

    std::shared_ptr<SceneObject> lightBulb;

    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SkyboxPass> skyboxPass;
    std::unique_ptr<BlitPass> blitPass;
    std::unique_ptr<SpritePass> spritePass;
    std::unique_ptr<GBufferPass> gBufferPass;
    std::unique_ptr<DeferredPass> deferredPass;

    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> specular;
    std::unique_ptr<Texture> normals;

    std::unique_ptr<Texture> LoadTextureFromPath(Graphics& graphics, const LPCWSTR& path)
    {
        Texture* tex = Texture::LoadTextureFromPath(graphics, path);
        return std::unique_ptr<Texture>(tex);
    }

    std::unique_ptr<Texture> CreateRenderTexture(Graphics& graphics, int width, int height, const std::string name, DXGI_FORMAT format)
    {
        Texture* tex = Texture::CreateTexture(graphics, width, height, name, format, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        tex->CreateRTV(graphics, format);
        tex->CreateSRV(graphics, format);
        return std::unique_ptr<Texture>(tex);
    }

public:
    void Start(Graphics& graphics) override
    {
        // Rendering Stuff
        auto& clientRect = graphics.m_ClientRect;
        unsigned int clientWidth = clientRect.right - clientRect.left;
        unsigned int clientHeight = clientRect.bottom - clientRect.top;
        colour = CreateRenderTexture(graphics, clientWidth, clientHeight, "Colour", DXGI_FORMAT_R8G8B8A8_UNORM);
        diffuse = CreateRenderTexture(graphics, clientWidth, clientHeight, "Diffuse", DXGI_FORMAT_R8G8B8A8_UNORM);
        normals = CreateRenderTexture(graphics, clientWidth, clientHeight, "Normals", DXGI_FORMAT_R11G11B10_FLOAT);
        specular = CreateRenderTexture(graphics, clientWidth, clientHeight, "Specular", DXGI_FORMAT_R8G8B8A8_UNORM);

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

        auto& colourTexture = *(colour.get());
        //auto& colourTexture = *(graphics.m_BackBuffer.get());
        forwardPass = std::make_unique<ForwardPass>(graphics, colourTexture);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\sky.dds");
        spritePass = std::make_unique<SpritePass>(graphics, colourTexture);
        blitPass = std::make_unique<BlitPass>(graphics, colourTexture, *(graphics.m_BackBuffer.get()));
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *specular.get(), *normals.get());
        deferredPass = std::make_unique<DeferredPass>(graphics, colourTexture, *diffuse.get(), *specular.get(), *normals.get());

        // Lighting
        auto lightColour = XMFLOAT4(Colors::LightSkyBlue);
        Light pointLight;
        pointLight.m_Color = XMFLOAT4(DirectX::Colors::Yellow);
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        Light dirLight;
        dirLight.m_Color = lightColour;
        dirLight.m_Direction = XMFLOAT4(-1, -1, 1, 0);
        dirLight.m_LightType = LightType::DirectionalLight;

        scene.lightManager
            .AddLight(pointLight)
            .AddLight(dirLight)
            .SetGlobalAmbient(XMFLOAT4(0.01, 0.01, 0.01, 0));

        // This shadow map is not tuned for sponza
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
        lightBulb = scene.CreateSceneObject("Light Bulb");
        lightBulbTex = LoadTextureFromPath(graphics, L"Data\\bulb.png");
        lightBulb->m_Transform
            .RotateEulerAngles(-0.5 * 3.142, 0, 0)
            .UniformScale(0.8);
        auto& spriteRenderer = lightBulb->m_SpriteRenderer;
        spriteRenderer.m_Sprite = lightBulbTex.get();
        spriteRenderer.m_IsEnabled = true;

        auto cube = scene.CreateSceneObject("Cube");
        cubeMesh = new Mesh(CubeVertices(), CubeIndices(), graphics);
        cubeMaterial
            .SetDiffuse(0.0, 0.5, 0.8);
        auto& cubeMeshRenderer = cube->m_MeshRenderer;
        cubeMeshRenderer.m_Mesh = cubeMesh;
        cubeMeshRenderer.m_Material = &cubeMaterial;
        cubeMeshRenderer.m_IsEnabled = true;
        cube->m_Transform
            .UniformScale(0.5)
            .RotateEulerAngles(0, 0.2, 0)
            .SetPosition(2, 0.5, -2);

        auto arrow = scene.CreateSceneObject("Arrow", cube->m_Index);
        arrowTex = LoadTextureFromPath(graphics, L"Data\\down-arrow.png");
        auto& arrowSpriteRenderer = arrow->m_SpriteRenderer;
        arrowSpriteRenderer.m_Sprite = arrowTex.get();
        arrowSpriteRenderer.m_IsEnabled = true;
        arrow->m_Transform
            .RotateEulerAngles(-0.5 * 3.142, 0, 0)
            .UniformScale(0.5)
            .SetPosition(0, 2, 0);

        auto plane = scene.CreateSceneObject("Plane");
        plane->m_Transform
            .UniformScale(5);
        planeMesh = new Mesh(QuadVertices(), QuadIndices(), graphics);
        brickTexture = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_COLOR.jpg");
        brickNormalMap = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_NORM.jpg");

        planeMaterial
            .SetDiffuse(1, 1, 1)
            .SetSpecular(0.5, 0.5, 0.5)
            .SetShininess(32)
            .UseDiffuseMap(brickTexture.get())
            .UseNormalMap(brickNormalMap.get());
        auto& planeMeshRenderer = plane->m_MeshRenderer;
        planeMeshRenderer.m_Mesh = planeMesh;
        planeMeshRenderer.m_Material = &planeMaterial;
        planeMeshRenderer.m_IsEnabled = true;

      /*  sponza = Model::LoadModelToScene("Data\\Models\\sponza\\sponza.obj", scene, graphics);
        auto sponzaObj = scene.GetSceneObjectByIndex(sponza->m_RootIndex);
        sponzaObj->m_Transform
            .UniformScale(0.01);*/

        stormtrooper = Model::LoadModelToScene("Data\\Models\\stormtrooper\\stormtrooper.obj", scene, graphics);

        scene.m_MainCamera.m_EyePosition = XMVectorSet(0, 1, -10, 1);

        scene.Start(graphics);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        static float phase = 0;
        phase += 45 * deltaTime;
        float zDisplacement = 6 * sin(XMConvertToRadians(phase));
        lightBulb->m_Transform.SetPosition(-2, 3, zDisplacement);

        Light& pointLight = scene.lightManager.GetLight(0);
        XMStoreFloat4(&(pointLight.m_Position), lightBulb->m_Transform.position);

        auto deviceContext = graphics.m_DeviceContext;

        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);

        scene.Update(graphics, input, deltaTime);
        //forwardPass->Render(graphics, scene);
        gBufferPass->Render(graphics, scene);
        deferredPass->Render(graphics, scene);
        skyboxPass->Render(graphics, scene);
        spritePass->Render(graphics, scene);
        blitPass->Render(graphics, scene);
    }

    void End()
    {
        if (planeMesh) delete planeMesh;
        if (cubeMesh) delete cubeMesh;
        if (stormtrooper) delete stormtrooper;
        if (sponza) delete sponza;
    }
};
