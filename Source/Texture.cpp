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
}

void Texture::Use(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetShaderResources(startSlot, 1, &m_Texture);
}

Texture::~Texture()
{
    SafeRelease(m_Texture);
}
