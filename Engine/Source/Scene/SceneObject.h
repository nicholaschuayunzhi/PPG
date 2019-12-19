#pragma once
#include "stdafx.h"
#include "Transform.h"
#include "Scene/Component/Rendering.h"

class Mesh;
class Texture;
class PhongMaterial;

class SceneObject
{
public:
    using Index = int;
    SceneObject(const std::string& name, SceneObject::Index index, SceneObject::Index parentIndex);
    SceneObject(SceneObject& copy) = delete;
    SceneObject(SceneObject&& obj) = default;
    SceneObject& operator= (const SceneObject&) = delete;

    std::string m_Name;
    Transform m_Transform;
    const Index m_Index;
    const Index  m_ParentIndex;
    std::vector<Index> m_ChildrenIndices;

    MeshRenderer m_MeshRenderer;
private:
    friend class Scene;
};

