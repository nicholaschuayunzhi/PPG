#pragma once
#include "stdafx.h"

class Graphics;

class Texture
{
public:
    Texture(ID3D11Texture2D* texture, const std::string& name);
    Texture(Texture&& texture);
    ~Texture();

    static Texture* CreateTexture(Graphics& graphics, int width, int height, const std::string& name, DXGI_FORMAT texFormat, UINT bindFlags, D3D11_SUBRESOURCE_DATA* data = NULL);
    static Texture* LoadTextureFromPath(Graphics& graphics, const LPCWSTR& texturePath);
    bool CreateSRV(Graphics& graphics, DXGI_FORMAT texFormat);
    bool CreateRTV(Graphics& graphics, DXGI_FORMAT texFormat);
    bool CreateDSV(Graphics& graphics, DXGI_FORMAT texFormat);

    void UseSRV(ID3D11DeviceContext* deviceContext, UINT startSlot);

    ID3D11Texture2D* m_Texture;
    ID3D11RenderTargetView* m_TextureRTV;
    ID3D11ShaderResourceView* m_TextureSRV;
    ID3D11DepthStencilView* m_TextureDSV;

    std::string m_Name;
private:
    Texture() = default;
};
