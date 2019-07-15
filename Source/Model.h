#pragma once
#include <map>
#include "stdafx.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "Mesh.h"
#include "Texture.h"

class Graphics;
class SceneObject;
class Scene;

class Model
{
public:
    SceneObject* m_Root;
    ~Model();
private:
    friend class ModelLoader;
    Model() = default;
    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
    std::vector<Texture*> m_Textures;
};
class ModelLoader
{
public:
    static Model* LoadModelToScene(std::string fileName, Scene& Scene, Graphics& graphics, SceneObject* parent = nullptr);
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, SceneObject* parent);
    void ProcessNode(aiNode* node, SceneObject* parent);
    Texture* loadTexture(aiTextureType type, aiMaterial* mat);
    std::map<std::string, Texture*> m_TextureMap;
    Model* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    Graphics& m_Graphics;
    std::string m_Directory;
    Model* m_Model;
    SceneObject* m_RootParent = nullptr;
    SceneObject* m_Root = nullptr;
};


