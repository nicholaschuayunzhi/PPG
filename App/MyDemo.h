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

    PBRMaterial planeMaterial;
    Mesh* planeMesh;

    Mesh* cubeMesh;
    Mesh* sphereMesh;
    PBRMaterial cubeMaterial;

    std::unique_ptr<Model> bobLamp;
    std::unique_ptr<Model> damagedHelmet;

    std::shared_ptr<SceneObject> lightBulb;

    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SkyboxPass> skyboxPass;
    std::unique_ptr<BlitPass> blitPass;
    std::unique_ptr<SpritePass> spritePass;
    std::unique_ptr<GBufferPass> gBufferPass;
    std::unique_ptr<DeferredPass> deferredPass;
    std::unique_ptr<SSAOPass> ssaoPass;
    std::unique_ptr<ToneMapPass> toneMapPass;

    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> metalRough;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> ambientOcclusion;
    std::unique_ptr<Texture> toneMappedColour;

    AnimationJob animJob;

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
        colour = CreateRenderTexture(graphics, clientWidth, clientHeight, "Colour", DXGI_FORMAT_R16G16B16A16_FLOAT);
        diffuse = CreateRenderTexture(graphics, clientWidth, clientHeight, "Diffuse", DXGI_FORMAT_R16G16B16A16_FLOAT);
        normals = CreateRenderTexture(graphics, clientWidth, clientHeight, "Normals", DXGI_FORMAT_R16G16B16A16_FLOAT);
        metalRough = CreateRenderTexture(graphics, clientWidth, clientHeight, "MetalRough", DXGI_FORMAT_R16G16B16A16_FLOAT);
        toneMappedColour = CreateRenderTexture(graphics, clientWidth, clientHeight, "Tone Mapped Colour", DXGI_FORMAT_R16G16B16A16_FLOAT);
        Texture* ao = Texture::CreateTexture(graphics, clientWidth, clientHeight, "Ambient Occlusion",
            DXGI_FORMAT_R16_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        ao->CreateRTV(graphics, DXGI_FORMAT_R16_UNORM);
        ao->CreateSRV(graphics, DXGI_FORMAT_R16_UNORM);
        ambientOcclusion = std::unique_ptr<Texture>(ao);
        brickTexture = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_COLOR.jpg");
        brickNormalMap = LoadTextureFromPath(graphics, L"Data\\Brick_Wall_014_NORM.jpg");

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

        auto& colourTexture = *(colour.get());
        auto& toneMappedTexture = *(toneMappedColour.get());
        forwardPass = std::make_unique<ForwardPass>(graphics, colourTexture);
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *metalRough.get(), *normals.get());
        deferredPass = std::make_unique<DeferredPass>(graphics, colourTexture, *diffuse.get(), *metalRough.get(), *normals.get());
        ssaoPass = std::make_unique<SSAOPass>(graphics, *ao, *(graphics.m_DepthStencilBuffer).get(), *normals.get());
        toneMapPass = std::make_unique<ToneMapPass>(graphics, colourTexture, toneMappedTexture);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\Malibu_Overlook_3k.hdr");
        //skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\studio_small.hdr");
        //skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\Theatre-Center_8k_TMap.jpg");
        //skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\sky.dds");
        spritePass = std::make_unique<SpritePass>(graphics, toneMappedTexture);
        blitPass = std::make_unique<BlitPass>(graphics, toneMappedTexture, *(graphics.m_BackBuffer.get()));
        // Lighting
        auto lightColour = XMFLOAT4(5.0f, 5.0f, 5.0f, 1.0f);

        Light pointLight;
        pointLight.m_Color = XMFLOAT4(1, 1, 0, 0);
        pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
        pointLight.m_LightType = LightType::PointLight;

        Light dirLight;
        dirLight.m_Color = lightColour;
        dirLight.m_Direction = XMFLOAT4(-1, -1, 1, 0);
        dirLight.m_LightType = LightType::DirectionalLight;

        Light spotLight;
        spotLight.m_Color = XMFLOAT4(Colors::Magenta);
        spotLight.m_Direction = XMFLOAT4(0, -1, 0, 0);
        spotLight.m_Position = XMFLOAT4(3, 2, 0, 0);
        spotLight.m_LightType = LightType::SpotLight;
        spotLight.m_SpotAngle = 3.142 / 4.0;

        scene.lightManager
            .AddLight(dirLight)
            .AddLight(pointLight)
            .AddLight(spotLight)
            .SetGlobalAmbient(XMFLOAT4(0.1, 0.1, 0.1, 1));

        // This shadow map is not tuned for sponza
        ShadowMapRenderDesc desc;
        desc.m_EyePosition = XMLoadFloat4(&(scene.lightManager.GetLight(0).m_Direction)) * -5;
        desc.m_LookAt = XMVectorSet(0, 0, 0, 1);
        desc.m_ViewWidth = 16;
        desc.m_ViewHeight = 12;
        desc.m_NearZ = 1;
        desc.m_FarZ = 20;
        desc.m_TextureWidth = clientRect.right - clientRect.left;
        desc.m_TextureHeight = clientRect.bottom - clientRect.top;
        scene.lightManager.SetLightWithShadows(graphics, 0, desc);

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
            .SetRoughness(0.8)
            .UseAlbedoMap(brickTexture.get())
            .UseNormalMap(brickNormalMap.get())
            .SetAlbedo(0.0, 0.3, 1.0);

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

        planeMaterial
            .SetAlbedo(0.8, 0.5, 0.5)
            .SetMetallic(0.2)
            .SetRoughness(0.7);
        auto& planeMeshRenderer = plane->m_MeshRenderer;
        planeMeshRenderer.m_Mesh = planeMesh;
        planeMeshRenderer.m_Material = &planeMaterial;
        planeMeshRenderer.m_IsEnabled = true;

        damagedHelmet = std::unique_ptr<Model>(Model::LoadModelToScene("Data\\Models\\DamagedHelmet\\glTF\\DamagedHelmet.gltf", scene, graphics));
        auto damagedHelmetObj = damagedHelmet->m_SceneObject;
        damagedHelmetObj->m_Transform
            .RotateEulerAngles(-3.412/2.0f, 0.f, 0.f)
            .Translate(0.f, 2.f, 0.f);

        bobLamp = std::unique_ptr<Model>(Model::LoadModelToScene("Data\\Models\\boblampclean\\boblampclean.md5mesh", scene, graphics));
        auto bobLampObj = bobLamp->m_SceneObject;
        bobLampObj->m_Transform
            .RotateEulerAngles(3.412f / 2.0f, 0.f, 0.f)
            .Translate(2.f, 0.f, 0.f)
            .UniformScale(0.05f);

        scene.m_MainCamera.m_EyePosition = XMVectorSet(0, 1, -10, 1);
        scene.Start(graphics);
        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);
        skyboxPass->GenerateCubeMap(graphics, scene);
        Texture* envMap = skyboxPass->GenerateEnvMap(graphics, scene);
        Texture* preFilter = skyboxPass->GenerateEnvPreFilter(graphics, scene);
        Texture* brdfLut = skyboxPass->GenerateBrdfLUT(graphics, scene);
        deferredPass->UseEnvMap(envMap, preFilter, brdfLut);
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        static float phase = 0;
        phase += 45 * deltaTime;
        float zDisplacement = 6 * sin(XMConvertToRadians(phase));
        lightBulb->m_Transform.SetPosition(-2, 3, zDisplacement);

        Light& pointLight = scene.lightManager.GetLight(1);
        XMStoreFloat4(&(pointLight.m_Position), lightBulb->m_Transform.position);

        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);

        scene.Update(graphics, input, deltaTime);


        animJob.Update(scene, deltaTime);
        //forwardPass->Render(graphics, scene);
        gBufferPass->Render(graphics, scene);
        ssaoPass->Render(graphics, scene);
        deferredPass->UseAmbientOcclusion(*ambientOcclusion.get());
        deferredPass->Render(graphics, scene);
        skyboxPass->Render(graphics, scene);
        toneMapPass->Render(graphics, scene);
        spritePass->Render(graphics, scene);
        blitPass->Render(graphics, scene);
    }

    void End()
    {
        if (planeMesh) delete planeMesh;
        if (cubeMesh) delete cubeMesh;
        if (sphereMesh) delete sphereMesh;
    }
};
