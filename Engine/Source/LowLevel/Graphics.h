#pragma once
#include "stdafx.h"
#include <atlbase.h>
#include <combaseapi.h>
using namespace DirectX;

class Window;

class Graphics
{
public:
    Graphics(HINSTANCE hInstance, BOOL vSync, Window& window);
    ~Graphics();
    void Present();
    void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
    void SetUp();

    ID3D11Buffer* CreateBuffer(UINT byteWidth, UINT bindFlags, const void* data);
    void UpdateBuffer(ID3D11Buffer* buffer, const void* resource);

    ID3D11Device* m_Device = nullptr;
    CComPtr<ID3D11Debug> m_Debug = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;
    IDXGISwapChain* m_SwapChain = nullptr;
    // Render target view for the back buffer of the swap chain.
    ID3D11RenderTargetView* m_RenderTargetView = nullptr;
    // Depth/stencil view for use as a depth buffer.
    ID3D11DepthStencilView* m_DepthStencilView = nullptr;
    // A texture to associate to the depth stencil view.
    ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
    // Define the functionality of the depth/stencil stages.
    ID3D11DepthStencilState* m_DepthStencilState = nullptr;
    // Define the functionality of the rasterizer stage.
    ID3D11RasterizerState* m_RasterizerState = nullptr;
    D3D11_VIEWPORT m_Viewport = { 0 };

    RECT m_ClientRect;
    BOOL m_enableVsync = TRUE;
private:
    DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
};
