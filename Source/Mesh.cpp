#include "stdafx.h"
#include "Graphics.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<WORD> indices, Graphics& graphics) :
    m_Vertices(std::move(vertices)),
    m_Indices(std::move(indices))
{
    m_VertexBuffer = graphics.CreateBuffer(sizeof(Vertex) * m_Vertices.size(), D3D11_BIND_VERTEX_BUFFER, m_Vertices.data());
    m_IndexBuffer = graphics.CreateBuffer(sizeof(WORD) * m_Indices.size(), D3D11_BIND_INDEX_BUFFER, m_Indices.data());
}

Mesh::~Mesh()
{
    SafeRelease(m_IndexBuffer);
    SafeRelease(m_VertexBuffer);
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext)
{
    const UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_VertexStride, &offset);
    deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexed(m_Indices.size(), 0, 0);
}
