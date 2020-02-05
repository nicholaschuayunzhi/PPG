#pragma once
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
#include "SkeletonLoader.h"
#include "Skeleton.h"

class ModelLoader
{
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, SceneObject::Index parentIndex);

    void ProcessMeshes();
    Mesh* GenerateMesh(aiMesh* mesh);
    PBRMaterial* GenerateMaterial(aiMesh* mesh);
    void ProcessBones(aiMesh* mesh, std::vector<Vertex>& vertices);

    Texture* loadTexture(aiTextureType type, aiMaterial* mat);
    std::map<std::string, Texture*> m_TextureMap;
    Model* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    Graphics& m_Graphics;
    std::string m_Directory;

    Model* m_Model;
    SkeletonLoader m_SkeletonLoader;
    friend class Model;

    bool m_HasBones = false;
};

ModelLoader::ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& directory, SceneObject::Index parentIndex) :
    m_AiScene(assimpScene),
    m_Scene(scene),
    m_Graphics(graphics),
    m_Directory(directory),
    m_SkeletonLoader(assimpScene)
{
    std::shared_ptr<SceneObject> object = m_Scene.CreateSceneObject(m_AiScene->mRootNode->mName.C_Str(), parentIndex);
    m_Model = new Model(object);
}

Model* ModelLoader::LoadModel()
{
    ProcessMeshes();
    if (m_HasBones)
    {
        Skeleton* skeleton = m_SkeletonLoader.GenerateSkeleton(m_AiScene->mRootNode);
        m_Model->m_Skeleton = skeleton;
        Animator& animator = m_Model->m_SceneObject->m_Animator;
        animator.m_IsEnabled = true;
        animator.m_Skeleton = skeleton;
    }
    return m_Model;
}

void ModelLoader::ProcessMeshes()
{
    std::shared_ptr<SceneObject> object = m_Model->m_SceneObject;
    for (UINT i = 0; i < m_AiScene->mNumMeshes; i++)
    {
        aiMesh* aimesh = m_AiScene->mMeshes[i];
        Mesh* mesh = GenerateMesh(aimesh);
        PBRMaterial* material = GenerateMaterial(aimesh);
        MeshRenderer& meshRenderer = object->m_MeshRenderer;
        meshRenderer.m_Materials.push_back(material);
        meshRenderer.m_Meshes.push_back(mesh);
        meshRenderer.m_IsEnabled = true;
    }
}

void ModelLoader::ProcessBones(aiMesh* mesh, std::vector<Vertex>& vertices)
{
    for (UINT i = 0; i < mesh->mNumBones; ++i)
    {
        m_HasBones = true;
        aiBone* bone = mesh->mBones[i];
        int boneId = m_SkeletonLoader.AddBone(bone);
        for (UINT j = 0; j < bone->mNumWeights; ++j)
        {
            aiVertexWeight vertWeight = bone->mWeights[j];
            UINT id = vertWeight.mVertexId;
            float weight = vertWeight.mWeight;

            // only support 4 weights
            for (UINT k = 0; k < 4; ++k)
            {
                Vertex& vertex = vertices[id];
                if (GetFloatAtIndex(vertex.BoneWeights, k) == 0.0)
                {
                    SetFloatAtIndex(vertex.BoneIds, k, boneId);
                    SetFloatAtIndex(vertex.BoneWeights, k, weight);
                    break;
                }
            }
        }
    }
}

Mesh* ModelLoader::GenerateMesh(aiMesh* aimesh)
{
    std::vector<Vertex> vertices;
    std::vector<WORD> indices;
    bool hasTexCoord = aimesh->mTextureCoords[0];

    for (UINT i = 0; i < aimesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = aimesh->mVertices[i].x;
        vertex.Position.y = aimesh->mVertices[i].y;
        vertex.Position.z = aimesh->mVertices[i].z;
        if (hasTexCoord)
        {
            vertex.TexCoord.x = aimesh->mTextureCoords[0][i].x;
            vertex.TexCoord.y = aimesh->mTextureCoords[0][i].y;
        }
        vertex.Normal.x = aimesh->mNormals[i].x;
        vertex.Normal.y = aimesh->mNormals[i].y;
        vertex.Normal.z = aimesh->mNormals[i].z;

        vertex.Tangent.x = aimesh->mTangents[i].x;
        vertex.Tangent.y = aimesh->mTangents[i].y;
        vertex.Tangent.z = aimesh->mTangents[i].z;

        vertex.Binormal.x = aimesh->mBitangents[i].x;
        vertex.Binormal.y = aimesh->mBitangents[i].y;
        vertex.Binormal.z = aimesh->mBitangents[i].z;
        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < aimesh->mNumFaces; i++)
    {
        aiFace face = aimesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back((WORD)face.mIndices[j]);
    }
    ProcessBones(aimesh, vertices);
    Mesh* mesh = new Mesh(vertices, indices, m_Graphics, false);
    m_Model->m_Meshes.push_back(mesh);
    return mesh;
}

PBRMaterial* ModelLoader::GenerateMaterial(aiMesh* mesh)
{
    PBRMaterial* material = new PBRMaterial();
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = m_AiScene->mMaterials[mesh->mMaterialIndex];
        Texture* specular = loadTexture(aiTextureType_SPECULAR, mat);

        Texture* albedo = loadTexture(aiTextureType_DIFFUSE, mat);
        if (albedo) material->UseAlbedoMap(albedo);

        Texture* normal = loadTexture(aiTextureType_NORMALS, mat);
        Texture* bump = loadTexture(aiTextureType_HEIGHT, mat);
        if (normal) material->UseNormalMap(normal);
        else if (bump) material->UseBumpMap(bump);

        aiColor3D colour;

        aiReturn res = mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour);
        if (res == aiReturn_SUCCESS)
            material->SetAlbedo(colour[0], colour[1], colour[2]);

        float shininess;
        res = mat->Get(AI_MATKEY_SHININESS, shininess);
        if (res == aiReturn_SUCCESS)
        {
            // convert shininess to roughness
            float roughness = sqrt(2.0f / (shininess + 2.0f));
            material->SetRoughness(roughness);
        }
    }
    else
    {
        material
            ->SetAlbedo(1, 0, 1);
    }
    m_Model->m_Materials.push_back(material);
    return material;
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

