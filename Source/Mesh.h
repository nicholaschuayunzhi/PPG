#pragma once
#include "stdafx.h"

using namespace DirectX;

// Only support this for now
struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
    XMFLOAT3 Tangent;
    XMFLOAT3 Binormal;

    Vertex() = default;
    Vertex(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT2 texC) :
        Position(pos), Normal(norm), TexCoord(texC), Tangent(), Binormal()
    {}
};

class Graphics;
class Texture;

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<WORD> indices, Graphics& graphics, bool calcTangents = true);
    Mesh() = default;
    Mesh(Mesh&& mesh);
    ~Mesh();
    void Draw(ID3D11DeviceContext* deviceContext);
private:
    std::vector<Vertex> m_Vertices;
    std::vector<WORD> m_Indices;
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;
    const UINT m_VertexStride = sizeof(Vertex);
};
