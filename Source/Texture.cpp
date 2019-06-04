#include "stdafx.h"
#include "Texture.h"
#include "Graphics.h"

Texture::Texture(LPCWSTR texturePath, Graphics& graphics)
{
    std::experimental::filesystem::path filePath(texturePath);
    if (!std::experimental::filesystem::exists(filePath))
    {
        throw std::exception("Texture::File not found");
    }

    ScratchImage image;
    TexMetadata metadata;
    HRESULT hr = LoadFromWICFile(texturePath, WIC_FLAGS_FORCE_RGB, &metadata, image);
    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to Load WIC File");
    }

    hr = CreateShaderResourceView(graphics.m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_Texture);
    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to create texture");
    }

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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

void Texture::Use(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShaderResources(0, 1, &m_Texture);
    deviceContext->PSSetSamplers(0, 1, &m_Sampler);
}

Texture::~Texture()
{
    SafeRelease(m_Texture);
    SafeRelease(m_Sampler);
}
