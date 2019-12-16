#include "stdafx.h"
#include "Texture.h"
#include "LowLevel/Graphics.h"
#include "DirectXTex.h"

ID3D11ShaderResourceView* Texture::nullSRV[] = { nullptr };

Texture::Texture(LPCWSTR texturePath, Graphics& graphics)
{
    std::filesystem::path filePath(texturePath);
    if (!std::filesystem::exists(filePath))
    {
        throw std::exception("Texture::File not found");
    }

    WCHAR ext[_MAX_EXT];
    _wsplitpath_s(texturePath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
    HRESULT hr;
    ScratchImage image;
    TexMetadata metadata;

    if (_wcsicmp(ext, L".dds") == 0)
    {
        hr = LoadFromDDSFile(texturePath, DDS_FLAGS_FORCE_RGB, &metadata, image);
    }
    else if (_wcsicmp(ext, L".tga") == 0)
    {
        hr = LoadFromTGAFile(texturePath, &metadata, image);
    }
    else
    {
        hr = LoadFromWICFile(texturePath, WIC_FLAGS_FORCE_RGB, &metadata, image);
    }

    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to Load Texture File");
    }

    hr = CreateShaderResourceView(graphics.m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_TextureSRV);
    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to create texture");
    }
}

// Creates empty texture that can be used as a render target and shader resource
Texture::Texture(int width, int height, Graphics& graphics, const std::string& name)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;

    // Initialize the render target texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    result = graphics.m_Device->CreateTexture2D(&textureDesc, NULL, &m_Texture);
    if (FAILED(result))
    {
        throw std::exception("Texture::Failed to create empty texture");
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    result = graphics.m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &m_TextureRTV);
    if (FAILED(result))
    {
        throw std::exception("Texture::Failed to create render target view");
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    // Create the shader resource view.
    result = graphics.m_Device->CreateShaderResourceView(m_Texture, &shaderResourceViewDesc, &m_TextureSRV);
    if (FAILED(result))
    {
        throw std::exception("Texture::Failed to create shader resource view");
    }
    SetDebugName(m_Texture, name);
    SetDebugName(m_TextureSRV, name + " SRV");
    SetDebugName(m_TextureRTV, name + " RTV");
}

Texture::Texture(Texture&& texture)
{
    m_Texture = texture.m_Texture;
    m_TextureSRV = texture.m_TextureSRV;
    m_TextureRTV = texture.m_TextureRTV;
    texture.m_Texture = NULL;
    texture.m_TextureSRV = NULL;
    texture.m_TextureRTV = NULL;
}

void Texture::Use(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetShaderResources(startSlot, 1, &m_TextureSRV);
}

void Texture::SetNullSrv(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetShaderResources(startSlot, 1, nullSRV);
}

Texture::~Texture()
{
    SafeRelease(m_Texture);
    SafeRelease(m_TextureSRV);
    SafeRelease(m_TextureRTV);
}
