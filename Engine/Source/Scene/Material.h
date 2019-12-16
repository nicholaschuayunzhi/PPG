#pragma once
#include "stdafx.h"

using namespace DirectX;

class Graphics;
class Texture;

_declspec(align(16)) struct MaterialInfo
{
    XMFLOAT4 m_Emissive = XMFLOAT4(0, 0, 0, 0);
    XMFLOAT4 m_Ambient = XMFLOAT4(0, 0, 0, 0);
    XMFLOAT4 m_Diffuse = XMFLOAT4(0, 0, 0, 0);
    XMFLOAT4 m_Specular = XMFLOAT4(0, 0, 0, 0);

    float m_Shininess = 0;
    int m_UseDiffuse = 0;
    int m_UseNormal = 0;
    int m_UseSpecular = 0;
};

class Material
{
public:
    Material() = default;
    ~Material();
    Material(Material& material) = delete;
    Material(Material&& material);

    void Use(ID3D11DeviceContext* deviceContext);
    void Update(Graphics& graphics);

    Material& SetEmissive(float r, float g, float b);
    Material& SetAmbient(float r, float g, float b);
    Material& SetDiffuse(float r, float g, float b);
    Material& SetSpecular(float r, float g, float b);
    Material& SetShininess(float n);
    Material& UseDiffuseMap(Texture* diffuse);
    Material& UseNormalMap(Texture* normal);
    Material& UseSpecularMap(Texture* specular);

    Texture* m_Diffuse = nullptr;
    Texture* m_Normal = nullptr;
    Texture* m_Specular = nullptr;

private:
    ID3D11Buffer* m_Buffer = nullptr;
    MaterialInfo m_MaterialInfo;
};
