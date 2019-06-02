#pragma once
#include "stdafx.h"

using namespace DirectX;

// Only support this for now
struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
};

class Graphics;

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<WORD> indices, Graphics& graphics);
    ~Mesh();
    void Draw(ID3D11DeviceContext* deviceContext);
    std::vector<Vertex> m_Vertices;
    std::vector<WORD> m_Indices;
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;
    const UINT m_VertexStride = sizeof(Vertex);
};
