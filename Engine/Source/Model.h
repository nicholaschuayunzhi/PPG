#pragma once
#include "stdafx.h"

#include "Mesh.h"
#include "Texture.h"

class Graphics;
class SceneObject;
class Scene;
class ModelLoader;

class Model
{
public:
    SceneObject* m_Root;
    ~Model();
    static Model* LoadModelToScene(std::string fileName, Scene& Scene, Graphics& graphics, SceneObject* parent = nullptr);
private:
    friend class ModelLoader;
    Model() = default;
    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
    std::vector<Texture*> m_Textures;
};
