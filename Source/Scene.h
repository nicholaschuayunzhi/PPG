#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"

class Material;
class Mesh;
class SceneObject;
class Graphics;

class Scene
{
public:
    Scene();
    ~Scene();
    Camera camera;
    LightManager lightManager;
    SceneObject* CreateSceneObject(std::string name, Mesh* mesh, Material* material, SceneObject* parent = nullptr);

    SceneObject* rootNode;
    void Start(Graphics& graphics);
    void Update(Graphics& graphics, Input input, float deltaTime);
    void Render(Graphics& graphics);
private:
    void DrawSceneRecursive(SceneObject* obj, XMMATRIX model, Graphics& graphics);
    ID3D11Buffer* modelBuffer;
};

class SceneObject
{
public:
    ~SceneObject();
    SceneObject(SceneObject& copy) = delete;
    SceneObject(SceneObject&& obj) = default;
    const SceneObject& m_Parent;

    Mesh* m_Mesh = nullptr;
    Material* m_Material = nullptr;
    Transform m_Transform;

    std::vector<SceneObject*> m_Children;
    std::string m_Name;
private:
    friend class Scene;
    SceneObject(std::string name, Mesh* mesh, Material* material, SceneObject& parent);
    SceneObject(std::string name);
};

