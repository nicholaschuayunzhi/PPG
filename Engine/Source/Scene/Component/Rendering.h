#pragma once

class Mesh;
class PhongMaterial;
class Texture;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    PhongMaterial* m_Material;
    Mesh* m_Mesh;
};

struct SpriteRenderer : public Component
{
    Texture* m_Sprite;
};
