#pragma once
#include "../Engine/Source/PPG.h"
#include <math.h>

class PBRSpheres : public Demo
{
private:
    Scene scene;

    std::unique_ptr<Sampler> linearSampler;
    std::unique_ptr<Sampler> pointSampler;

    std::unique_ptr<Model> sphere;
    std::vector<PBRMaterial*> mats;

    std::unique_ptr<SkyboxPass> skyboxPass;
    std::unique_ptr<BlitPass> blitPass;
    std::unique_ptr<SpritePass> spritePass;
    std::unique_ptr<GBufferPass> gBufferPass;
    std::unique_ptr<DeferredPass> deferredPass;
    std::unique_ptr<ForwardPass> forwardPass;
    std::unique_ptr<SSAOPass> ssaoPass;
    std::unique_ptr<ToneMapPass> toneMapPass;

    std::unique_ptr<Texture> colour;
    std::unique_ptr<Texture> diffuse;
    std::unique_ptr<Texture> metalRough;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> ambientOcclusion;
    std::unique_ptr<Texture> toneMappedColour;

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

        linearSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
        pointSampler = std::make_unique<Sampler>(graphics, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

        auto& colourTexture = *(colour.get());
        auto& toneMappedTexture = *(toneMappedColour.get());
        forwardPass = std::make_unique<ForwardPass>(graphics, colourTexture);
        gBufferPass = std::make_unique<GBufferPass>(graphics, *diffuse.get(), *metalRough.get(), *normals.get());
        deferredPass = std::make_unique<DeferredPass>(graphics, colourTexture, *diffuse.get(), *metalRough.get(), *normals.get());
        ssaoPass = std::make_unique<SSAOPass>(graphics, *ao, *(graphics.m_DepthStencilBuffer).get(), *normals.get());
        toneMapPass = std::make_unique<ToneMapPass>(graphics, colourTexture, toneMappedTexture);
        skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\Malibu_Overlook_3k.hdr");
        //skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\sky.dds");
        //skyboxPass = std::make_unique<SkyboxPass>(graphics, colourTexture, L"Data\\Theatre-Center_8k_TMap.jpg");

        spritePass = std::make_unique<SpritePass>(graphics, toneMappedTexture);
        blitPass = std::make_unique<BlitPass>(graphics, toneMappedTexture, *(graphics.m_BackBuffer.get()));
        // Lighting


        XMFLOAT4 lightPos[4] = {
            { -10.0f, 10.0f, -10.0f, 1.0f },
            { 10.0f, 10.0f, -10.0f, 1.0f },
            { -10.0f, -10.0f, -10.0f, 1.0f },
            { 10.0f, -10.0f, -10.0f, 1.0f }
        };

        for (int i = 0; i < 4; ++i)
        {
            Light light;
            light.m_Position = lightPos[i];
            light.m_Color = XMFLOAT4(10.0f, 10.0f, 10.0f, 0.0f);
            light.m_LightType = LightType::PointLight;
            scene.lightManager.AddLight(light);
        }

        scene.lightManager
            .SetGlobalAmbient(XMFLOAT4(0.1, 0.1, 0.1, 1));

        sphere = std::unique_ptr<Model>(Model::LoadModelToScene("Data\\Models\\sphere.obj", scene, graphics));
        sphere->m_SceneObject->m_Transform.UniformScale(0.5);
        sphere->m_SceneObject->m_MeshRenderer.m_IsEnabled = false;

        const int rows = 6;
        const int cols = 6;
        const float spacing = 2;
        Mesh* sphereMesh = sphere->m_SceneObject->m_MeshRenderer.m_Mesh;

        for (int row = 0; row <= rows; ++row)
        {
            float metallic = (float)row / (float)rows;
            for (int col = 0; col <= cols; ++col)
            {
                float roughness = std::max((float)col / (float)cols, 0.05f);
                PBRMaterial* mat = new PBRMaterial();
                mat->SetAlbedo(1, 0, 0);
                mat->SetMetallic(metallic);
                mat->SetRoughness(roughness);
                mats.emplace_back(mat);

                auto ball = scene.CreateSceneObject("ball");
                auto& meshRenderer = ball->m_MeshRenderer;
                meshRenderer.m_IsEnabled = true;
                meshRenderer.m_Mesh = sphereMesh;
                meshRenderer.m_Material = mat;
                ball->m_Transform.Translate(
                    (col - (cols / 2)) * spacing,
                    (row - (rows / 2)) * spacing,
                    0.0f
                ).UniformScale(0.3);
            }
        }

        scene.m_MainCamera.m_EyePosition = XMVectorSet(0, 1, -10, 1);
        scene.Start(graphics);
        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);
        skyboxPass->GenerateCubeMap(graphics, scene);
        Texture* envMap = skyboxPass->GenerateEnvMap(graphics, scene);
        Texture* specMap = skyboxPass->GenerateEnvPreFilter(graphics, scene);
        Texture* brdfLut = skyboxPass->GenerateBrdfLUT(graphics, scene);
        deferredPass->UseEnvMap(envMap, specMap, brdfLut);
        deferredPass->UseAmbientOcclusion(*ambientOcclusion.get());
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {
        auto deviceContext = graphics.m_DeviceContext;
        linearSampler->Use(deviceContext, 0);
        pointSampler->Use(deviceContext, 1);

        scene.Update(graphics, input, deltaTime);
        //forwardPass->Render(graphics, scene);
        gBufferPass->Render(graphics, scene);
        ssaoPass->Render(graphics, scene);
        deferredPass->Render(graphics, scene);
        skyboxPass->Render(graphics, scene);
        toneMapPass->Render(graphics, scene);
        spritePass->Render(graphics, scene);
        blitPass->Render(graphics, scene);
    }

    void End()
    {
        for (PBRMaterial* mat : mats)
        {
            delete mat;
        }
        mats.clear();
    }
};
