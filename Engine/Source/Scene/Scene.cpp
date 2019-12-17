#include "stdafx.h"
#include "Scene/Scene.h"
#include "Scene/Model.h"
#include "Scene/Material.h"
#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "LowLevel/Graphics.h"

Scene::Scene()
{
    //rootNode = new SceneObject("Root");
    CreateSceneObject("Root", nullptr, nullptr, 0);
}

Scene::~Scene()
{
    SafeRelease(m_ModelBuffer);
}

void Scene::Start(Graphics& graphics)
{
    camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
    lightManager
        .SetEyePosition(camera.m_EyePosition)
        .Update(graphics);

    m_ModelBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
}

void Scene::UpdateModelRecursive(SceneObject& obj, XMMATRIX model)
{
    model = XMMatrixMultiply(obj.m_Transform.GetLocalModel(), model);
    obj.m_Transform.SetModel(model);
    for (auto objIndex : obj.m_ChildrenIndices)
        UpdateModelRecursive(GetSceneObjectByIndex(objIndex), model);
}

void Scene::Update(Graphics& graphics, Input input, float deltaTime)
{
    lightManager.Update(graphics);
    lightManager.RenderAnyShadowMap(graphics, *this);

    camera.HandleMovement(input, deltaTime);
    camera.UpdateView(graphics, camera.CalculateView());
    camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
    lightManager.SetEyePosition(camera.m_EyePosition);

    for (auto objIndex : m_Objects[0].m_ChildrenIndices)
        UpdateModelRecursive(GetSceneObjectByIndex(objIndex), XMMatrixIdentity());
}

void Scene::UseModel(Graphics& graphics)
{
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->VSSetConstantBuffers(0, 1, &m_ModelBuffer);
}

void Scene::UpdateModel(Graphics& graphics, const XMMATRIX& model)
{
    graphics.UpdateBuffer(m_ModelBuffer, &model);
}

Scene::ObjectIndex Scene::CreateSceneObject(const std::string& name, Mesh* mesh, Material* material, ObjectIndex parentIndex /*= 0*/)
{
    ObjectIndex index = m_Objects.size();
    m_Objects.push_back({ name, mesh, material, index, parentIndex });
    auto& parentObj = GetSceneObjectByIndex(parentIndex);
    if (parentObj.m_Index != index)
        parentObj.m_ChildrenIndices.push_back(index);
    return index;
}

SceneObject& Scene::GetSceneObjectByIndex(ObjectIndex index)
{
    if (index < m_Objects.size())
    {
        return m_Objects[index];
    }
    return m_Objects[0];
}

SceneObject::SceneObject(const std::string& name, Mesh* mesh, Material* material, 
                         Scene::ObjectIndex index, Scene::ObjectIndex parentIndex) :
    m_Name(name),
    m_Mesh(mesh),
    m_Material(material),
    m_Index(index),
    m_ParentIndex(parentIndex),
    m_ChildrenIndices()
{
}
