#pragma once
#include <vector>

class Mesh;
class PBRMaterial;
class Texture;
class Skeleton;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    std::vector<PBRMaterial*> m_Materials;
    std::vector<Mesh*> m_Meshes;
};

struct SpriteRenderer : public Component
{
    Texture* m_Sprite;
};

struct Animator : public Component
{
    Skeleton* m_Skeleton;
    float m_TimeElapsed = 0;
    UINT m_AnimIndexChosen = 0;
    XMMATRIX m_FinalTransforms[50];
};
