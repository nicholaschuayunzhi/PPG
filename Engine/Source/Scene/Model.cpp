#include "stdafx.h"

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

class ModelLoader
{
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, SceneObject::Index parentIndex);
    void ProcessNode(aiNode* node, SceneObject::Index parentIndex);
    Texture* loadTexture(aiTextureType type, aiMaterial* mat);
    std::map<std::string, Texture*> m_TextureMap;
    Model* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    Graphics& m_Graphics;
    std::string m_Directory;

    Model* m_Model;
    SceneObject::Index m_RootParentIndex;
    SceneObject::Index m_RootIndex;

    friend class Model;
};

ModelLoader::ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, SceneObject::Index parentIndex) :
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
    auto object = m_Scene.CreateSceneObject(m_AiScene->mRootNode->mName.C_Str(), m_RootParentIndex);
    ProcessNode(m_AiScene->mRootNode, object->m_Index);
    m_Model->m_RootIndex = object->m_Index;
    return m_Model;
}

void ModelLoader::ProcessNode(aiNode* node, SceneObject::Index parentIndex)
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

        PBRMaterial* materialPtr = new PBRMaterial();
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = m_AiScene->mMaterials[mesh->mMaterialIndex];
            Texture* specular = loadTexture(aiTextureType_SPECULAR, mat);

            Texture* albedo = loadTexture(aiTextureType_DIFFUSE, mat);
            if (albedo) materialPtr->UseAlbedoMap(albedo);

            Texture* normal = loadTexture(aiTextureType_NORMALS, mat);
            Texture* bump = loadTexture(aiTextureType_HEIGHT, mat);
            if (normal) materialPtr->UseNormalMap(normal);
            else if (bump) materialPtr->UseBumpMap(bump);

            aiColor3D colour;

            aiReturn res = mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour);
            if (res == aiReturn_SUCCESS)
                materialPtr->SetAlbedo(colour[0], colour[1], colour[2]);

            float shininess;
            res = mat->Get(AI_MATKEY_SHININESS, shininess);
            if (res == aiReturn_SUCCESS)
            {
                // convert shininess to roughness
                float roughness = sqrt(2.0f / (shininess + 2.0f));
                materialPtr->SetRoughness(roughness);
            }
        }
        else
        {
            materialPtr
                ->SetAlbedo(1, 0, 1);
        }
        m_Model->m_Materials.push_back(materialPtr);
        auto object = m_Scene.CreateSceneObject(node->mName.C_Str(), parentIndex);
        parentIndex = object->m_Index;
        auto& meshRenderer = object->m_MeshRenderer;
        meshRenderer.m_Material = materialPtr;
        meshRenderer.m_Mesh = meshPtr;
        meshRenderer.m_IsEnabled = true;
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
        LPCWSTR path = stemp.c_str();
        auto it = m_TextureMap.find(textureName);
        if (it != m_TextureMap.end())
        {
            texture = it->second;
        }
        else
        {
            texture = Texture::LoadTextureFromPath(m_Graphics, path);
            m_TextureMap.emplace(textureName, texture);
            m_Model->m_Textures.push_back(texture);
        }
    }
    return texture;
}

Model* Model::LoadModelToScene(std::string fileName, Scene& scene, Graphics& graphics, SceneObject::Index parentIndex /*= 0*/)
{
    Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
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

    for (PBRMaterial* mat : m_Materials)
        delete mat;

    for (Texture* tex : m_Textures)
        delete tex;
}
