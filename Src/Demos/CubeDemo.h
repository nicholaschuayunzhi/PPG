#pragma once
#include "../stdafx.h"

#include "Demo.h"
#include "../Camera.h"
#include "../Graphics.h"
#include "../Mesh.h"
#include "../Shader.h"


class CubeDemo : public Demo
{
public:
    void Start(Graphics& graphics) override
    {
        std::vector<Vertex> vertices;
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }); // 0
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }); // 1
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }); // 2
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }); // 3
        vertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }); // 4
        vertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }); // 5
        vertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }); // 6
        vertices.push_back({ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) });  // 7

        std::vector<WORD> indices =
        {
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7
        };

        mesh = new Mesh(std::move(vertices), std::move(indices), graphics);
        shader = new Shader(L"VertexShader.cso", L"PixelShader.cso", graphics);
        XMMATRIX projection = camera.CalculateProjection(graphics.m_ClientRect);
        graphics.UpdateBuffer(shader->m_MVPBuffer[2], &projection);
        XMMATRIX view = camera.CalculateView();
        graphics.UpdateBuffer(shader->m_MVPBuffer[1], &view);
    }

    void Update(Graphics& graphics, float deltaTime) override
    {
        static float angle = 0.0f;
        angle += 90.0f * deltaTime;
        XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
        XMMATRIX model = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        graphics.UpdateBuffer(shader->m_MVPBuffer[0], &model);
        graphics.Draw(*mesh, *shader);
    }

    void End()
    {
        if (mesh) delete mesh;
        if (shader) delete shader;
    }

  private:
    Camera camera;
    Mesh* mesh;
    Shader* shader;
  };
