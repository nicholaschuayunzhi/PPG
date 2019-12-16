#include "stdafx.h"
#include "Sampler.h"
#include "LowLevel/Graphics.h"

Sampler::Sampler(Graphics& graphics)
{
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.MaxLOD = FLT_MAX;

    graphics.m_Device->CreateSamplerState(&samplerDesc, &m_Sampler);
}

void Sampler::Use(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetSamplers(startSlot, 1, &m_Sampler);
}

Sampler::~Sampler()
{
    SafeRelease(m_Sampler);
}
