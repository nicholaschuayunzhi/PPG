#pragma once
#include <map>
#include "stdafx.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "Mesh.h"
#include "Texture.h"

class Graphics;

class Model
{
public:
    Model(std::string fileName, Graphics& graphics);
    void Draw(ID3D11DeviceContext* deviceContext);
private:
    std::vector<Mesh> m_Meshes;
    std::vector<Texture*> m_Diffuse;
    std::vector<Texture*> m_Normal;
    std::map<std::string, Texture> m_Textures;

    const aiScene* m_Scene;
    Graphics& m_Graphics;
    std::string m_Directory;

    void processNode(aiNode* node);
    Texture* loadTexture(aiTextureType type, aiMaterial* mat);
};
