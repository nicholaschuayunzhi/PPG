#pragma once
#include "stdafx.h"

class Graphics;

class Sampler
{
public:
    Sampler(Graphics& graphics);
    Sampler(Graphics& graphics, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode);
    ~Sampler();
    void Use(ID3D11DeviceContext* deviceContext, UINT startSlot);
    ID3D11SamplerState* m_Sampler;
};
