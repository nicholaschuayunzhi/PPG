#include "stdafx.h"

#include "ModelLoader.h"
#include <map>
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "LowLevel/Graphics.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Material/Material.h"
#include "Scene/Scene.h"
#include "Skeleton.h"

Model* Model::LoadModelToScene(std::string fileName, Scene& scene, Graphics& graphics, SceneObject::Index parentIndex /*= 0*/)
{
    Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_ConvertToLeftHanded
    );
    if (assimpScene == NULL)
        throw std::exception("ModelLoader::Model file not found");
    ModelLoader ml = ModelLoader(assimpScene, scene, graphics, fileName, parentIndex);
    Model* model = ml.LoadModel();
    return ml.m_Model;
}

Model::Model(std::shared_ptr<SceneObject>& sceneObject) :
    m_SceneObject(sceneObject)
{
}

Model::~Model()
{
    for (Mesh* mesh : m_Meshes)
        delete mesh;

    for (PBRMaterial* mat : m_Materials)
        delete mat;

    for (Texture* tex : m_Textures)
        delete tex;

    if (m_Skeleton != nullptr)
        delete m_Skeleton;
}

