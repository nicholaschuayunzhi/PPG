#include "stdafx.h"
#include "Texture.h"
#include "LowLevel/Graphics.h"
#include "DirectXTex.h"

Texture* Texture::CreateTexture(Graphics& graphics, int width, int height, const std::string& name,
    DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data /*= NULL*/)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    // Initialize the render target texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = texFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    ID3D11Texture2D* texturePtr;
    HRESULT result = graphics.m_Device->CreateTexture2D(&textureDesc, data, &texturePtr);
    if (FAILED(result))
    {
        return nullptr;
    }

    return new Texture(texturePtr, name);
}

Texture* Texture::CreateTextureCube(Graphics& graphics, int size, const std::string& name,
    DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data /*= NULL*/)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = size;
    textureDesc.Height = size;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6; // 6 faces
    textureDesc.Format = texFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* texturePtr;
    HRESULT result = graphics.m_Device->CreateTexture2D(&textureDesc, data, &texturePtr);
    if (FAILED(result))
    {
        return nullptr;
    }

    return new Texture(texturePtr, name);
}

Texture* Texture::LoadTextureFromPath(Graphics& graphics, const LPCWSTR& texturePath)
{
    std::filesystem::path filePath(texturePath);
    if (!std::filesystem::exists(filePath))
    {
        return nullptr;
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
    else if (_wcsicmp(ext, L".hdr") == 0)
    {
        hr = LoadFromHDRFile(texturePath, &metadata, image);
    }
    else
    {
        hr = LoadFromWICFile(texturePath, DDS_FLAGS_NONE, &metadata, image);
    }

    if (FAILED(hr))
    {
        return nullptr;
    }

    Texture* tex = new Texture();

    hr = CreateShaderResourceView(graphics.m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &tex->m_TextureSRV);
    if (FAILED(hr))
    {
        throw std::exception("Texture::Failed to create texture");
    }
    return tex;
}


Texture::Texture(ID3D11Texture2D* texture, const std::string& name) :
    m_Texture(texture),
    m_Name(name)
{
    SetDebugName(m_Texture, name);
}

bool Texture::CreateSRV(Graphics& graphics, DXGI_FORMAT texFormat, D3D11_SRV_DIMENSION viewDimension /*= D3D11_SRV_DIMENSION_TEXTURE2D*/)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    shaderResourceViewDesc.Format = texFormat;
    shaderResourceViewDesc.ViewDimension = viewDimension;

    if (viewDimension == D3D11_SRV_DIMENSION_TEXTURECUBE)
    {
        shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
        shaderResourceViewDesc.TextureCube.MipLevels = 1;
    }
    else
    {
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;
    }
    // Create the shader resource view.
    HRESULT result = graphics.m_Device->CreateShaderResourceView(m_Texture, &shaderResourceViewDesc, &m_TextureSRV);
    SetDebugName(m_TextureSRV, m_Name + " SRV");
    return SUCCEEDED(result);
}

bool Texture::CreateRTV(Graphics& graphics, DXGI_FORMAT texFormat)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = texFormat;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    ID3D11RenderTargetView* rtv;
    HRESULT result = graphics.m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &rtv);
    SetDebugName(rtv, m_Name + " RTV");
    m_TextureRTVs.push_back(rtv);
    return SUCCEEDED(result);
}

bool Texture::CreateTextureCubeRTVs(Graphics& graphics, DXGI_FORMAT texFormat)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = texFormat;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    renderTargetViewDesc.Texture2DArray.MipSlice = 0;
    renderTargetViewDesc.Texture2DArray.ArraySize = 1;
    for (UINT i = 0; i < 6; ++i)
    {
        renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
        ID3D11RenderTargetView* rtv;
        HRESULT result = graphics.m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &rtv);
        SetDebugName(rtv, m_Name + " RTV " + std::to_string(i));
        m_TextureRTVs.push_back(rtv);
        if (FAILED(result))
        {
            return false;
        }
    }
    return true;
}

bool Texture::CreateDSV(Graphics& graphics, DXGI_FORMAT texFormat)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = texFormat;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    HRESULT result = graphics.m_Device->CreateDepthStencilView(m_Texture, &depthStencilViewDesc, &m_TextureDSV);
    SetDebugName(m_TextureDSV, m_Name + " DSV");
    return SUCCEEDED(result);
}

ID3D11RenderTargetView* Texture::GetRTV(UINT index /*= 0*/)
{
    return m_TextureRTVs[index];
}

void Texture::UseSRV(ID3D11DeviceContext* deviceContext, UINT startSlot)
{
    deviceContext->PSSetShaderResources(startSlot, 1, &m_TextureSRV);
}

Texture::Texture(Texture&& texture)
{
    m_Texture = texture.m_Texture;
    m_TextureSRV = texture.m_TextureSRV;
    m_TextureRTVs = texture.m_TextureRTVs;
    m_TextureDSV = texture.m_TextureDSV;
    m_Name = std::move(texture.m_Name);

    texture.m_Texture = NULL;
    texture.m_TextureSRV = NULL;
    texture.m_TextureRTVs.clear();
    texture.m_TextureDSV = NULL;
}


Texture::~Texture()
{
    SafeRelease(m_Texture);
    SafeRelease(m_TextureSRV);
    for (ID3D11RenderTargetView* rtv : m_TextureRTVs)
    {
        SafeRelease(rtv);
    }
    m_TextureRTVs.clear();
    SafeRelease(m_TextureDSV);
}
