#include "stdafx.h"
#include "Scene/Scene.h"
#include "Scene/Model.h"
#include "Scene/Material.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "LowLevel/Graphics.h"

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
    camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
    lightManager.SetEyePosition(camera.m_EyePosition);
}

void Scene::UseModel(Graphics& graphics)
{
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->VSSetConstantBuffers(0, 1, &modelBuffer);
}

void Scene::UpdateModel(Graphics& graphics, XMMATRIX& model)
{
    graphics.UpdateBuffer(modelBuffer, &model);
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
