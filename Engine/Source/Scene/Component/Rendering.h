#pragma once
#include <vector>

class Mesh;
class PBRMaterial;
class Texture;
class Skeleton;
class Animator;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    PBRMaterial* m_Material;
    Mesh* m_Mesh;
    Animator* m_Animator = nullptr;
};

struct SpriteRenderer : public Component
{
    Texture* m_Sprite = nullptr;
};

struct Animator : public Component
{
    Skeleton* m_Skeleton = nullptr;
    float m_TimeElapsed = 0;
    UINT m_AnimIndexChosen = 0;
    XMMATRIX m_FinalTransforms[50];
};
