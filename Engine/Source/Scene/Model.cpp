#include "stdafx.h"

#include <map>
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "LowLevel/Graphics.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Material.h"
#include "Scene/Scene.h"

class ModelLoader
{
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, Scene::ObjectIndex parentIndex);
    void ProcessNode(aiNode* node, Scene::ObjectIndex parentIndex);
    Texture* loadTexture(aiTextureType type, aiMaterial* mat);
    std::map<std::string, Texture*> m_TextureMap;
    Model* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    Graphics& m_Graphics;
    std::string m_Directory;

    Model* m_Model;
    Scene::ObjectIndex m_RootParentIndex;
    Scene::ObjectIndex m_RootIndex;

    friend class Model;
};

ModelLoader::ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, Scene::ObjectIndex parentIndex) :
    m_AiScene(assimpScene),
    m_Scene(scene),
    m_Graphics(graphics),
    m_Directory(directory),
    m_RootParentIndex(parentIndex)
{
    m_Model = new Model();
}

Model* ModelLoader::LoadModel()
{
    ProcessNode(m_AiScene->mRootNode, m_RootParentIndex);
    m_Model->m_RootIndex = m_RootIndex;
    return m_Model;
}

void ModelLoader::ProcessNode(aiNode* node, Scene::ObjectIndex parentIndex)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = m_AiScene->mMeshes[node->mMeshes[i]];
        std::vector<Vertex> vertices;
        std::vector<WORD> indices;
        bool hasTexCoord = mesh->mTextureCoords[0];

        for (UINT i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;
            if (hasTexCoord)
            {
                vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
            }
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;

            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;

            vertex.Binormal.x = mesh->mBitangents[i].x;
            vertex.Binormal.y = mesh->mBitangents[i].y;
            vertex.Binormal.z = mesh->mBitangents[i].z;
            vertices.push_back(vertex);
        }

        for (UINT i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
                indices.push_back((WORD)face.mIndices[j]);
        }
        Mesh* meshPtr = new Mesh(vertices, indices, m_Graphics, false);
        m_Model->m_Meshes.push_back(meshPtr);

        Material* materialPtr = new Material();
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = m_AiScene->mMaterials[mesh->mMaterialIndex];
            Texture* diffuse = loadTexture(aiTextureType_DIFFUSE, mat);
            Texture* normal = loadTexture(aiTextureType_HEIGHT, mat);
            Texture* specular = loadTexture(aiTextureType_SPECULAR, mat);
            if (diffuse) materialPtr->UseDiffuseMap(diffuse);
            if (normal) materialPtr->UseNormalMap(normal);
            if (specular) materialPtr->UseSpecularMap(specular);
        }
        else
        {
            materialPtr
                ->SetAmbient(1, 0, 1)
                .SetDiffuse(1, 0, 1)
                .SetSpecular(1, 0, 1);
        }
        materialPtr->Update(m_Graphics);
        m_Model->m_Materials.push_back(materialPtr);
        parentIndex = m_Scene.CreateSceneObject("", meshPtr, materialPtr, parentIndex);
        if (m_RootIndex == 0)
            m_RootIndex = parentIndex;
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], parentIndex);
    }
}

Texture* ModelLoader::loadTexture(aiTextureType type, aiMaterial* mat)
{
    bool hasTex = mat->GetTextureCount(type) > 0;
    Texture* texture = nullptr;
    if (hasTex)
    {
        aiString str;
        mat->GetTexture(type, 0, &str);
        std::string textureName = str.C_Str();
        textureName = m_Directory + textureName;
        std::wstring stemp = std::wstring(textureName.begin(), textureName.end());
        LPCWSTR sw = stemp.c_str();
        auto it = m_TextureMap.find(textureName);
        if (it != m_TextureMap.end())
        {
            texture = it->second;
        }
        else
        {
            texture = new Texture(sw, m_Graphics);
            m_TextureMap.emplace(textureName, texture);
            m_Model->m_Textures.push_back(texture);
        }
    }
    return texture;
}

Model* Model::LoadModelToScene(std::string fileName, Scene& scene, Graphics& graphics, Scene::ObjectIndex parentIndex /*= 0*/)
{
    Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_MakeLeftHanded |
        aiProcess_FlipUVs
    );
    if (assimpScene == NULL)
        throw std::exception("ModelLoader::Model file not found");
    std::string directory = fileName.substr(0, fileName.find_last_of('\\') + 1);
    ModelLoader ml = ModelLoader(assimpScene, scene, graphics, directory, parentIndex);
    Model* model = ml.LoadModel();
    return ml.m_Model;
}

Model::~Model()
{
    for (Mesh* mesh : m_Meshes)
        delete mesh;

    for (Material* mat : m_Materials)
        delete mat;

    for (Texture* tex : m_Textures)
        delete tex;
}
