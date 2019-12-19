#pragma once

class Mesh;
class PhongMaterial;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    PhongMaterial* m_Material;
    Mesh* m_Mesh;
};
