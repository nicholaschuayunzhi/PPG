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
    SafeRelease(m_ViewBuffer);
    SafeRelease(m_ProjBuffer);
}

void Scene::Start(Graphics& graphics)
{
    lightManager
        .SetEyePosition(m_MainCamera.m_EyePosition)
        .Update(graphics);

    auto clientRect = graphics.m_ClientRect;
    float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);
    m_MainCamera.m_AspectRatio = clientWidth / clientHeight;

    m_ModelBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
    m_ViewBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
    m_ProjBuffer = graphics.CreateBuffer(sizeof(XMMATRIX), D3D11_BIND_CONSTANT_BUFFER, &XMMatrixIdentity());
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

    m_MainCamera.HandleMovement(input, deltaTime);
    lightManager.SetEyePosition(m_MainCamera.m_EyePosition);

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

void Scene::UseCamera(Graphics& graphics, Camera& camera)
{
    graphics.UpdateBuffer(m_ViewBuffer, &(camera.CalculateView()));
    graphics.UpdateBuffer(m_ProjBuffer, &(camera.CalculateProjection()));
    auto deviceContext = graphics.m_DeviceContext;
    deviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
    deviceContext->VSSetConstantBuffers(2, 1, &m_ProjBuffer);
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
