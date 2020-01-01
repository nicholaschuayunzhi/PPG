#pragma once

class Mesh;
class PBRMaterial;
class Texture;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    PBRMaterial* m_Material;
    Mesh* m_Mesh;
};

struct SpriteRenderer : public Component
{
    Texture* m_Sprite;
};
