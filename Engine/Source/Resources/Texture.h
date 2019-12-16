#pragma once
#include "stdafx.h"

class Graphics;

class Texture
{
public:
    Texture(LPCWSTR texturePath, Graphics& graphics);
    Texture(int width, int height, Graphics& graphics, const std::string& name);
    Texture(Texture&& texture);
    ~Texture();
    void Use(ID3D11DeviceContext* deviceContext, UINT startSlot);
    static void SetNullSrv(ID3D11DeviceContext* deviceContext, UINT startSlot);
    ID3D11Texture2D* m_Texture;
    ID3D11RenderTargetView* m_TextureRTV;
    ID3D11ShaderResourceView* m_TextureSRV;
private:
    static ID3D11ShaderResourceView* nullSRV[1];
};
