#include "stdafx.h"
#include "Scene/Scene.h"
#include "Scene/Material/Material.h"
#include "Resources/Mesh.h"
#include "LowLevel/Graphics.h"

Scene::Scene()
{
    CreateSceneObject("Root", 0);
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

void Scene::UpdateModelRecursive(SceneObject::Index idx, XMMATRIX model)
{
    auto obj = GetSceneObjectByIndex(idx);
    model = XMMatrixMultiply(obj->m_Transform.GetLocalModel(), model);
    obj->m_Transform.SetModel(model);
    for (auto objIndex : obj->m_ChildrenIndices)
        UpdateModelRecursive(objIndex, model);
}

void Scene::Update(Graphics& graphics, Input input, float deltaTime)
{
    lightManager.Update(graphics);
    lightManager.RenderAnyShadowMap(graphics, *this);

    camera.HandleMovement(input, deltaTime);
    camera.UpdateView(graphics, camera.CalculateView());
    camera.UpdateProjection(graphics, camera.CalculateProjection(graphics.m_ClientRect));
    lightManager.SetEyePosition(camera.m_EyePosition);

    for (auto objIndex : m_Objects[0]->m_ChildrenIndices)
        UpdateModelRecursive(objIndex, XMMatrixIdentity());
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

std::shared_ptr<SceneObject> Scene::CreateSceneObject(const std::string& name, SceneObject::Index parentIndex /*= 0*/)
{
    SceneObject::Index index = m_Objects.size();
    m_Objects.push_back(std::make_shared<SceneObject>( name, index, parentIndex ));
    auto parentObj = GetSceneObjectByIndex(parentIndex);
    if (parentObj->m_Index != index)
        parentObj->m_ChildrenIndices.push_back(index);
    return GetSceneObjectByIndex(index);
}

std::shared_ptr<SceneObject> Scene::GetSceneObjectByIndex(SceneObject::Index index)
{
    if (index < m_Objects.size())
    {
        return m_Objects[index];
    }
    return m_Objects[0];
}
