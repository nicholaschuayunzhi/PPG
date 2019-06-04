#pragma once
#include "stdafx.h"

class Graphics;

class Sampler
{
public:
    Sampler(Graphics& graphics);
    ~Sampler();
    void Use(ID3D11DeviceContext* deviceContext, UINT startSlot);
    ID3D11SamplerState* m_Sampler;
};
