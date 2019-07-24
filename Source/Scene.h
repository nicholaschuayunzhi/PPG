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
class SkyBox;

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
    void LoadSkyBox(Graphics& graphics, LPCWSTR fileName);
private:
    void DrawSkyBox(Graphics& graphics);
    void DrawSceneRecursive(SceneObject* obj, XMMATRIX model, Graphics& graphics);
    ID3D11Buffer* modelBuffer;
    std::unique_ptr<SkyBox> skyBox;
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

class SkyBox
{
public:
    SkyBox(Graphics& graphics, LPCWSTR fileName, float size = 50);
private:
    friend class Scene;
    std::unique_ptr<Mesh> m_Mesh;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<Shader> m_Shader;
    XMMATRIX m_Model;
};
