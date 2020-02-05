#pragma once
#include "stdafx.h"

#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Scene.h"
#include "Skeleton.h"

class Graphics;
class SceneObject;
class ModelLoader;

class Model
{
public:
    std::shared_ptr<SceneObject> m_SceneObject;
    Model(std::shared_ptr<SceneObject>& sceneObject);
    ~Model();
    Skeleton* m_Skeleton;
    static Model* LoadModelToScene(std::string fileName, Scene& Scene, Graphics& graphics, SceneObject::Index parentIndex = 0);
private:
    friend class ModelLoader;
    std::vector<Mesh*> m_Meshes;
    std::vector<PBRMaterial*> m_Materials;
    std::vector<Texture*> m_Textures;
};

