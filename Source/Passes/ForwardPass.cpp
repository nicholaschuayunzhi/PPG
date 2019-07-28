#include "stdafx.h"
#include "ForwardPass.h"
#include "Graphics.h"
#include "Scene.h"
#include "Mesh.h"
#include "Shader.h"

ForwardPass::ForwardPass(Graphics& graphics)
{
    shader = std::make_unique<Shader>(L"VertexShader.cso", L"PixelShader.cso", graphics);
}

void ForwardPass::Render(Graphics& graphics, Scene& scene)
{
    auto deviceContext = graphics.m_DeviceContext;
    scene.camera.Use(deviceContext);
    scene.lightManager.Use(deviceContext, 1); // should be linked to material + shader
    scene.UseModel(graphics); // not so good...
    shader->Use(deviceContext);

    _Graphics = &graphics;
    _Scene = &scene;
    for (auto sceneObj : scene.rootNode->m_Children) // root node is empty
        DrawSceneRecursive(*sceneObj, XMMatrixIdentity());
}

void ForwardPass::DrawSceneRecursive(SceneObject& obj, XMMATRIX model)
{
    model = XMMatrixMultiply(obj.m_Transform.GetModel(), model);
    _Scene->UpdateModel(*_Graphics, model);
    obj.m_Mesh->Draw(_Graphics->m_DeviceContext, obj.m_Material);
    for (auto child : obj.m_Children)
    {
        DrawSceneRecursive(*child, model);
    }
}
