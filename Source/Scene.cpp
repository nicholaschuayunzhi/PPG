#include "stdafx.h"
#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Model.h"
#include "Shader.h"

Scene::Scene()
{
    rootNode = new SceneObject("Root");
}

Scene::~Scene()
{
    delete rootNode;
    SafeRelease(modelBuffer);
}

void Scene::Start(Graphics& graphics)
{
    camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
    lightManager
        .SetEyePosition(camera.m_EyePosition)
        .Update(graphics);

    modelBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
}

void Scene::Update(Graphics& graphics, Input input, float deltaTime)
{
    camera.HandleMovement(input, deltaTime);
    camera.UpdateView(graphics, camera.CalculateView());
    lightManager.SetEyePosition(camera.m_EyePosition);
}

void Scene::Render(Graphics& graphics)
{
    auto deviceContext = graphics.m_DeviceContext;
    camera.Use(deviceContext);
    deviceContext->VSSetConstantBuffers(0, 1, &modelBuffer);
    lightManager.Use(deviceContext, 1); // should be linked to material + shader
    DrawSceneRecursive(rootNode, XMMatrixIdentity(), graphics);
    DrawSkyBox(graphics);
  }

void Scene::DrawSceneRecursive(SceneObject* obj, XMMATRIX model, Graphics& graphics)
{
    if (obj == nullptr) return;
    if (&obj->m_Parent != obj) // not root node
    {
        model = XMMatrixMultiply(obj->m_Transform.GetModel(), model);
        graphics.UpdateBuffer(modelBuffer, &model);
        obj->m_Mesh->Draw(graphics.m_DeviceContext, obj->m_Material);
    }

    for (SceneObject* child : obj->m_Children)
    {
        DrawSceneRecursive(child, model, graphics);
    }
}

void Scene::LoadSkyBox(Graphics& graphics, LPCWSTR fileName)
{
    skyBox = std::make_unique<SkyBox>(graphics, fileName);
}

void Scene::DrawSkyBox(Graphics& graphics)
{
    if (skyBox)
    {
        auto model = XMMatrixMultiply(skyBox->m_Model, XMMatrixTranslationFromVector(camera.m_EyePosition));
        graphics.UpdateBuffer(modelBuffer, &model);
        auto devCon = graphics.m_DeviceContext;
        skyBox->m_Shader->Use(devCon);
        skyBox->m_Texture->Use(devCon, 0);
        skyBox->m_Mesh->Draw(devCon, nullptr);
        Texture::SetNullSrv(devCon, 0);
    }
}

SceneObject* Scene::CreateSceneObject(std::string name, Mesh* mesh, Material* material, SceneObject* parent /*= nullptr*/)
{
    if (parent == nullptr)
        parent = rootNode;
    SceneObject* obj = new SceneObject(name, mesh, material, *parent);
    return obj;
}


SceneObject::SceneObject(std::string name) :
    m_Parent(*this),
    m_Name(name)
{
}

SceneObject::SceneObject(std::string name, Mesh* mesh, Material* material, SceneObject& parent) :
    m_Mesh(mesh),
    m_Material(material),
    m_Parent(parent),
    m_Transform(),
    m_Children(),
    m_Name(name)
{
    parent.m_Children.push_back(this);
}

SceneObject::~SceneObject()
{
    for (SceneObject* child : m_Children)
    {
        delete child;
    }
}

SkyBox::SkyBox(Graphics& graphics, LPCWSTR fileName, float size /*=50*/)
{
    std::vector<Vertex> cubeVertices;
    cubeVertices.reserve(24);

    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) });

    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) });

    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f) });
    cubeVertices.push_back({ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f) });

    std::vector<WORD> skyboxIndices =
    {
        0,  2,  1,  0,  3,  2,
        4,  6,  5,  4,  7,  6,
        8,  10, 9,  8, 11, 10,
       12, 14, 13, 12, 15, 14,
       16, 18, 17, 16, 19, 18,
       20, 22, 21, 20, 23, 22
    };

    m_Mesh = std::make_unique<Mesh>(std::move(cubeVertices), std::move(skyboxIndices), graphics);
    m_Texture = std::make_unique<Texture>(fileName, graphics);
    m_Shader = std::make_unique<Shader>(L"Skybox.vs.cso", L"Skybox.ps.cso", graphics);
    m_Model = XMMatrixScaling(50, 50, 50);
}
