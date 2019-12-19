#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "SceneObject.h"

class Mesh;
class PhongMaterial;
class Graphics;

class Scene
{
public:
    Scene();
    ~Scene();
    PerspectiveCamera m_MainCamera;
    LightManager lightManager;
    std::shared_ptr<SceneObject> CreateSceneObject(const std::string& name, SceneObject::Index parentIndex = 0);
    std::shared_ptr<SceneObject> GetSceneObjectByIndex(SceneObject::Index index);
    std::vector<std::shared_ptr<SceneObject>> m_Objects;
    void Start(Graphics& graphics);
    void Update(Graphics& graphics, Input input, float deltaTime);
    void UseModel(Graphics& graphics);
    void UpdateModel(Graphics& graphics, const XMMATRIX& model);
    void UseCamera(Graphics& graphics, Camera& camera);
private:
    void UpdateModelRecursive(SceneObject::Index objIdx, XMMATRIX model);
    ID3D11Buffer* m_ModelBuffer;
    ID3D11Buffer* m_ViewBuffer;
    ID3D11Buffer* m_ProjBuffer;
};


