#pragma once
#include "stdafx.h"

class Graphics;

class Texture
{
public:
    Texture(LPCWSTR texturePath, Graphics& graphics);
    ~Texture();
    void Use(ID3D11DeviceContext* deviceContext);
    ID3D11ShaderResourceView* m_Texture;
    ID3D11SamplerState* m_Sampler;
};
