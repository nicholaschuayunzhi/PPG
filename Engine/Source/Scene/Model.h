#pragma once
#include "stdafx.h"

#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Scene.h"

class Graphics;
class SceneObject;
class ModelLoader;

class Model
{
public:
    Scene::ObjectIndex m_RootIndex;
    ~Model();
    static Model* LoadModelToScene(std::string fileName, Scene& Scene, Graphics& graphics, Scene::ObjectIndex parentIndex = 0);
private:
    friend class ModelLoader;
    Model() = default;
    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
    std::vector<Texture*> m_Textures;
};
