#pragma once
#include "stdafx.h"
#include <atlbase.h>
#include <combaseapi.h>
using namespace DirectX;

class Window;
class Texture;

class Graphics
{
public:
    Graphics(HINSTANCE hInstance, BOOL vSync, Window& window);
    ~Graphics();
    void Present();
    void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
    void ClearRenderTargetView(ID3D11RenderTargetView* rtv, const FLOAT clearColor[4]);
    void SetUp();

    ID3D11Buffer* CreateBuffer(UINT byteWidth, UINT bindFlags, const void* data);
    void UpdateBuffer(ID3D11Buffer* buffer, const void* resource);
    void SetRenderTarget(Texture& texture, bool enableDepthTest = true);
    void UnbindRenderTargetView();
    void UnbindShaderResourceView(UINT startSlot);

    ID3D11Device* m_Device = nullptr;
    CComPtr<ID3D11Debug> m_Debug = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;
    IDXGISwapChain* m_SwapChain = nullptr;

    std::unique_ptr<Texture> m_BackBuffer;
    std::unique_ptr<Texture> m_DepthStencilBuffer;

    ID3D11DepthStencilState* m_DepthStencilState = nullptr;
    ID3D11RasterizerState* m_RasterizerState = nullptr;
    ID3D11BlendState* m_AlphaBlendState = nullptr;
    D3D11_VIEWPORT m_Viewport = { 0 };

    RECT m_ClientRect;
    BOOL m_enableVsync = TRUE;
private:
    DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
};
