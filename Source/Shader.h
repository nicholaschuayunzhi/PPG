#pragma once
#include "stdafx.h"

class Graphics;

class Shader
{
public:
    Shader(LPCWSTR vertexPath, LPCWSTR pixelPath, Graphics& graphicss);
    ~Shader();
    void Use(ID3D11DeviceContext* deviceContext);
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    ID3D11InputLayout* m_InputLayout = nullptr;
};
