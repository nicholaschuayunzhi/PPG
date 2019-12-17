#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"

class Mesh;
class Shader;
class Texture;
class Material;
class SceneObject;
class Graphics;

class Scene
{
public:
    Scene();
    ~Scene();

    using ObjectIndex = int;
    Camera camera;
    LightManager lightManager;
    ObjectIndex CreateSceneObject(const std::string& name, Mesh* mesh, Material* material, ObjectIndex parentIndex = 0);
    SceneObject& GetSceneObjectByIndex(ObjectIndex index);

    std::vector<SceneObject> m_Objects;
    void Start(Graphics& graphics);
    void Update(Graphics& graphics, Input input, float deltaTime);
    void UseModel(Graphics& graphics);
    void UpdateModel(Graphics& graphics, const XMMATRIX& model);
private:
    void UpdateModelRecursive(SceneObject& obj, XMMATRIX model);
    ID3D11Buffer* m_ModelBuffer;
};

class SceneObject
{
public:
    SceneObject(SceneObject& copy) = delete;
    SceneObject(SceneObject&& obj) = default;
    SceneObject& operator= (const SceneObject&) = delete;

    std::string m_Name;
    Transform m_Transform;
    const Scene::ObjectIndex m_Index;
    const Scene::ObjectIndex m_ParentIndex;
    std::vector<Scene::ObjectIndex> m_ChildrenIndices;

    Mesh* m_Mesh = nullptr;
    Material* m_Material = nullptr;
private:
    friend class Scene;
    SceneObject(const std::string& name, Mesh* mesh, Material* material, Scene::ObjectIndex index,  Scene::ObjectIndex parentIndex);
};
