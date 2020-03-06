#pragma once
#include "stdafx.h"
#include "Resources/Texture.h"

using namespace DirectX;

_declspec(align(16)) struct PBRMaterialInfo
{
    XMFLOAT4 m_Albedo = XMFLOAT4(0, 0, 0, 0);
    float m_Metallic = 0.0f;
    float m_Roughness = 1.0f;
    int m_UseAlbedo = 0;
    int m_UseOccRoughMetal = 0;
    int m_UseAOMap = 0;
    int m_UseEmissive = 0;
    int m_NormalState = 0;
    int m_ConvertToLinear = 0;
};

class PBRMaterial
{
public:
    PBRMaterial() = default;
    PBRMaterial(PBRMaterial& material) = delete;
    PBRMaterial(PBRMaterial&& material);

    PBRMaterial& SetAlbedo(float r, float g, float b);
    PBRMaterial& SetMetallic(float n);
    PBRMaterial& SetRoughness(float n);
    PBRMaterial& UseAlbedoMap(Texture* albedo);
    PBRMaterial& UseNormalMap(Texture* normal);
    PBRMaterial& UseBumpMap(Texture* bump);
    PBRMaterial& UseOccRoughMetal(Texture* occMetalRough);
    PBRMaterial& UseAoMap(Texture* aoMap);
    PBRMaterial& UseEmissiveMap(Texture* emissive);
    PBRMaterial& ConvertToLinear(bool convertToLinear);

    Texture* m_Albedo = nullptr;
    Texture* m_Normal = nullptr;
    Texture* m_OccRoughMetal = nullptr;
    Texture* m_AoMap = nullptr;
    Texture* m_Emissive = nullptr;

    PBRMaterialInfo m_MaterialInfo;
};

