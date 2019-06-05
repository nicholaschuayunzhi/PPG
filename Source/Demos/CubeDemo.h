#pragma once
#include "../stdafx.h"

#include "Demo.h"
#include "../Camera.h"
#include "../Graphics.h"
#include "../Mesh.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../Sampler.h"


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
        texture = new Texture(L"..\\..\\Data\\DirectX9.png", graphics);
        sampler = new Sampler(graphics);

        XMMATRIX projection = camera.CalculateProjection(graphics.m_ClientRect);
        graphics.UpdateBuffer(shader->m_MVPBuffer[2], &projection);
        graphics.UpdateBuffer(shader->m_MVPBuffer[0], &XMMatrixIdentity());
    }

    void Update(Graphics& graphics, Input input, float deltaTime) override
    {

        if (input != Key::NONE)
        {
            float x = 0;
            float z = 0;
            if (input & Key::UP)
                z += 1;
            if (input & Key::DOWN)
                z -= 1;
            if (input & Key::LEFT)
                x -= 1;
            if (input & Key::RIGHT)
                x += 1;
            camera.m_EyePosition += XMVectorSet(x, 0, z, 0) * camera.m_Speed * deltaTime;
            camera.m_LookAt = camera.m_EyePosition + XMVectorSet(0, 0, 1, 0);
        }

        XMMATRIX view = camera.CalculateView();
        graphics.UpdateBuffer(shader->m_MVPBuffer[1], &view);

        auto deviceContext = graphics.m_DeviceContext;

        shader->Use(deviceContext);
        texture->Use(deviceContext, 0);
        sampler->Use(deviceContext, 0);
        cubeMesh->Draw(deviceContext);
    }

    void End()
    {
        if (cubeMesh) delete cubeMesh;
        if (shader) delete shader;
        if (texture) delete texture;
        if (sampler) delete sampler;
    }

  private:
    Camera camera;
    Mesh* cubeMesh;
    Shader* shader;
    Texture* texture;
    Sampler* sampler;
  };
