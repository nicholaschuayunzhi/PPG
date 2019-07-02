#include "stdafx.h"
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Texture.h"

Model::Model(std::string fileName, Graphics& graphics) :
    m_Graphics(graphics)
{
    Assimp::Importer importer;
    m_Scene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals  |
        aiProcess_CalcTangentSpace |
        aiProcess_MakeLeftHanded |
        aiProcess_FlipUVs
    );
    if (m_Scene == NULL)
        throw std::exception("Model::Model file not found");
    m_Directory = fileName.substr(0, fileName.find_last_of('\\') + 1);
    processNode(m_Scene->mRootNode);
}

void Model::processNode(aiNode* node)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
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

        m_Meshes.emplace_back(vertices, indices, m_Graphics, false);
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* mat = m_Scene->mMaterials[mesh->mMaterialIndex];
            Texture* diffuseTex = loadTexture(aiTextureType_DIFFUSE, mat);
            m_Diffuse.push_back(diffuseTex);
            Texture* normalMap = loadTexture(aiTextureType_HEIGHT, mat);
            m_Normal.push_back(normalMap);
        }
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
}

Texture* Model::loadTexture(aiTextureType type, aiMaterial* mat)
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
        auto it = m_Textures.find(textureName);
        if (it != m_Textures.end())
        {
            texture = &(it->second);
        }
        else
        {
            auto result = m_Textures.emplace(textureName, Texture(sw, m_Graphics));
            texture = &(result.first->second);
        }
    }
    return texture;
}

void Model::Draw(ID3D11DeviceContext* deviceContext)
{
    for (int i = 0; i < m_Meshes.size(); ++i)
    {
        Mesh& mesh = m_Meshes[i];
        Texture* diffuse = m_Diffuse[i];
        if (diffuse)
            diffuse->Use(deviceContext, 0);
        Texture* normal = m_Normal[i];
        if (normal)
            normal->Use(deviceContext, 1);
        mesh.Draw(deviceContext);
    }
}
