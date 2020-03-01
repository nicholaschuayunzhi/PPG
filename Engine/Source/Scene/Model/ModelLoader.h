#pragma once
#include "stdafx.h"

#include <map>
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/pbrmaterial.h"
#include "LowLevel/Graphics.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Scene/Material/Material.h"
#include "Scene/Scene.h"
#include "SkeletonLoader.h"
#include "Skeleton.h"

class ModelLoader
{
    enum class LoadType
    {
        UNKNOWN,
        OBJ,
        GLTF,
    };
private:
    ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& fileName, SceneObject::Index parentIndex);

    void ProcessMeshes();
    Mesh* GenerateMesh(aiMesh* mesh);
    PBRMaterial* GenerateMaterial(aiMesh* mesh);
    void ProcessBones(aiMesh* mesh, std::vector<Vertex>& vertices);
    void GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex);

    Texture* loadTexture(aiMaterial* mat, aiTextureType type, unsigned int index = 0);
    std::map<std::string, Texture*> m_TextureMap;
    Model* LoadModel();

    const aiScene* m_AiScene;
    Scene& m_Scene;
    LoadType m_LoadType = LoadType::UNKNOWN;
    Graphics& m_Graphics;
    std::string m_Directory;

    Model* m_Model;
    SkeletonLoader m_SkeletonLoader;
    Animator* m_Animator;
    friend class Model;

    bool m_HasBones = false;
};

ModelLoader::ModelLoader(const aiScene* assimpScene, Scene& scene, Graphics& graphics, std::string& fileName, SceneObject::Index parentIndex) :
    m_AiScene(assimpScene),
    m_Scene(scene),
    m_Graphics(graphics),
    m_SkeletonLoader(assimpScene)
{
    std::filesystem::path filePath = fileName;
    m_Directory = filePath.parent_path().string() + "\\";
    if (filePath.has_extension())
    {
        std::filesystem::path extension = filePath.extension();
        if (extension == ".obj")
        {
            m_LoadType = LoadType::OBJ;
        }
        else if (extension == ".gltf")
        {
            m_LoadType = LoadType::GLTF;
        }
    }
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
        m_Animator = &animator;
    }

    GenerateSceneObjectHierarchy(m_AiScene->mRootNode, true, m_Model->m_SceneObject->m_Index);
    return m_Model;
}

void ModelLoader::ProcessMeshes()
{
    std::shared_ptr<SceneObject> object = m_Model->m_SceneObject;
    for (UINT i = 0; i < m_AiScene->mNumMeshes; i++)
    {
        aiMesh* aimesh = m_AiScene->mMeshes[i];
        GenerateMesh(aimesh);
        GenerateMaterial(aimesh);
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

void ModelLoader::GenerateSceneObjectHierarchy(aiNode* node, bool isRoot, int parentIndex)
{
    if (node->mNumMeshes > 0)
    {
        for (UINT i = 0; i < node->mNumMeshes; ++i)
        {
            std::shared_ptr<SceneObject> object = isRoot ? m_Model->m_SceneObject : m_Scene.CreateSceneObject(node->mName.C_Str(), parentIndex);
            unsigned int meshId = node->mMeshes[i];
            Mesh* mesh = m_Model->m_Meshes[meshId];
            PBRMaterial* material = m_Model->m_Materials[meshId];
            MeshRenderer& meshRenderer = object->m_MeshRenderer;
            meshRenderer.m_IsEnabled = true;
            meshRenderer.m_Mesh = mesh;
            meshRenderer.m_Material = material;
            object->m_Transform.SetLocalModel(XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)));

            if (m_HasBones)
            {
                meshRenderer.m_Animator = m_Animator;
            }
            isRoot = false;
            parentIndex = object->m_Index;
        }
    }

    for (UINT i = 0; i < node->mNumChildren; ++i)
    {
        GenerateSceneObjectHierarchy(node->mChildren[i], false, parentIndex);
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
        Texture* normal = loadTexture(mat, aiTextureType_NORMALS);
        Texture* bump = loadTexture(mat, aiTextureType_HEIGHT);
        if (normal) material->UseNormalMap(normal);
        else if (bump) material->UseBumpMap(bump);

        if (m_LoadType == LoadType::GLTF)
        {
            Texture* albedo = loadTexture(mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE);
            if (albedo) material->UseAlbedoMap(albedo);

            Texture* metalRough = loadTexture(mat, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE);
            if (metalRough) material->UseOccRoughMetal(metalRough);

            float metallic;
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic) == AI_SUCCESS)
            {
                material->SetMetallic(metallic);
            }
            float roughness;
            if (mat->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
            {
                material->SetRoughness(roughness);
            }
        }
        else
        {
            Texture* albedo = loadTexture(mat, aiTextureType_DIFFUSE);
            if (albedo) material->UseAlbedoMap(albedo);

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
            else
            {
                material->SetRoughness(0.5f);
            }
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

Texture* ModelLoader::loadTexture(aiMaterial* mat, aiTextureType type, unsigned int index)
{
    bool hasTex = mat->GetTextureCount(type) > 0;
    Texture* texture = nullptr;
    if (hasTex)
    {
        aiString str;
        mat->GetTexture(type, index, &str);
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

