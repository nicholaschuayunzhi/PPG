#include "stdafx.h"
#include "Window.h"
#include "LowLevel/Graphics.h"
#include "Resources/Texture.h"

using namespace DirectX;

ID3D11ShaderResourceView* nullSRV[] = { nullptr };
ID3D11RenderTargetView* nullRTV[] = { nullptr };

Graphics::Graphics(HINSTANCE hInstance, BOOL vSync, Window& window)
{
    m_ClientRect = window.GetWindowRect();

    // Compute the exact client dimensions. This will be used
    // to initialize the render targets for our swap chain.
    unsigned int clientWidth = m_ClientRect.right - m_ClientRect.left;
    unsigned int clientHeight = m_ClientRect.bottom - m_ClientRect.top;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = clientWidth;
    swapChainDesc.BufferDesc.Height = clientHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, vSync);
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.OutputWindow = window.m_WindowHandle;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed = TRUE;
    UINT createDeviceFlags = 0;
#if _DEBUG
    createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    // These are the feature levels that we will accept.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // This will be the feature level that
    // is used to create our device and swap chain.
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
        D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &featureLevel,
        &m_DeviceContext);

    if (hr == E_INVALIDARG)
    {
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
            D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &featureLevel,
            &m_DeviceContext);
    }

    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to Create Device and Swap Chain");
    }

    hr = m_Device->QueryInterface(IID_PPV_ARGS(&m_Debug));
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to Create Debug");
    }

    // Next initialize the back buffer of the swap chain and associate it to a
    // render target view.
    ID3D11Texture2D* backBuffer;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBuffer);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to Get Back Buffer");
    }

    m_BackBuffer = std::make_unique<Texture>(backBuffer, "Back Buffer");
    m_BackBuffer->CreateRTV(*this, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

    Texture* depthStencil = Texture::CreateTexture(*this, clientWidth, clientHeight, "Depth Stencil Buffer",
        DXGI_FORMAT_R24G8_TYPELESS, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
    depthStencil->CreateDSV(*this, DXGI_FORMAT_D24_UNORM_S8_UINT);
    depthStencil->CreateSRV(*this, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
    m_DepthStencilBuffer = std::unique_ptr<Texture>(depthStencil);

    // Setup depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;

    hr = m_Device->CreateDepthStencilState(&depthStencilStateDesc, &m_DepthStencilState);

    // Setup rasterizer state.
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state object.
    hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create Rasterize State");
    }

    // Initialize the viewport to occupy the entire client area.
    m_Viewport.Width = static_cast<float>(clientWidth);
    m_Viewport.Height = static_cast<float>(clientHeight);
    m_Viewport.TopLeftX = 0.0f;
    m_Viewport.TopLeftY = 0.0f;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;

    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    m_Device->CreateBlendState(&BlendState, &m_AlphaBlendState);
}

ID3D11Buffer* Graphics::CreateBuffer(UINT byteWidth, UINT bindFlags, const void* data)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.BindFlags = bindFlags;
    bufferDesc.ByteWidth = byteWidth;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ID3D11Buffer* constantBuffer;
    HRESULT hr;
    if (data == nullptr)
    {
        hr = m_Device->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
    }
    else
    {
        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
        resourceData.pSysMem = data;
        hr = m_Device->CreateBuffer(&bufferDesc, &resourceData, &constantBuffer);
    }

    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create Buffer");
    }
    return constantBuffer;
}

void Graphics::UpdateBuffer(ID3D11Buffer* buffer, const void* resource)
{
    m_DeviceContext->UpdateSubresource(buffer, 0, nullptr, resource, 0, 0);
}

void Graphics::SetRenderTarget(Texture& texture, bool enableDepthTest /*= true*/)
{
    SetRenderTarget(texture.GetRTV(), enableDepthTest);
}

void Graphics::SetRenderTarget(ID3D11RenderTargetView* rtv, bool enableDepthTest /*= true*/)
{
    ID3D11DepthStencilView* dsv = enableDepthTest ? m_DepthStencilBuffer->m_TextureDSV : NULL;
    m_DeviceContext->OMSetRenderTargets(1, &(rtv), dsv);
}

void Graphics::UnbindShaderResourceView(UINT startSlot)
{
    m_DeviceContext->PSSetShaderResources(startSlot, 1, nullSRV);
}

void Graphics::UnbindRenderTargetView()
{
    m_DeviceContext->OMSetRenderTargets(_countof(nullRTV), nullRTV, nullptr);
}

void Graphics::SetUp()
{
    m_DeviceContext->RSSetState(m_RasterizerState);
    m_DeviceContext->RSSetViewports(1, &m_Viewport);
    SetRenderTarget(*(m_BackBuffer.get()));
    m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
    m_DeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void Graphics::Present()
{
    if (m_enableVsync)
    {
        m_SwapChain->Present(1, 0);
    }
    else
    {
        m_SwapChain->Present(0, 0);
    }
}

void Graphics::Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil)
{
    ClearRenderTargetView(m_BackBuffer->GetRTV(), clearColor);
    ClearDepthStencil(clearDepth, clearStencil);
}

void Graphics::ClearRenderTargetView(ID3D11RenderTargetView* rtv, const FLOAT clearColor[4])
{
    m_DeviceContext->ClearRenderTargetView(rtv, clearColor);
}

void Graphics::ClearDepthStencil(FLOAT clearDepth, UINT8 clearStencil)
{
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilBuffer->m_TextureDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

// This function was inspired by:
// http://www.rastertek.com/dx11tut03.html
DXGI_RATIONAL Graphics::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
{
    DXGI_RATIONAL refreshRate = { 0, 1 };
    if (vsync)
    {
        IDXGIFactory* factory;
        IDXGIAdapter* adapter;
        IDXGIOutput* adapterOutput;
        DXGI_MODE_DESC* displayModeList;

        // Create a DirectX Graphics interface factory.
        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Could not create DXGIFactory instance."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to create DXGIFactory.");
        }

        hr = factory->EnumAdapters(0, &adapter);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to enumerate adapters."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to enumerate adapters.");
        }

        hr = adapter->EnumOutputs(0, &adapterOutput);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to enumerate adapter outputs."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to enumerate adapter outputs.");
        }

        UINT numDisplayModes;
        hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to query display mode list.");
        }

        displayModeList = new DXGI_MODE_DESC[numDisplayModes];
        assert(displayModeList);

        hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to query display mode list.");
        }

        // Now store the refresh rate of the monitor that matches the width and height of the requested screen.
        for (UINT i = 0; i < numDisplayModes; ++i)
        {
            if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
            {
                refreshRate = displayModeList[i].RefreshRate;
            }
        }

        delete[] displayModeList;
        SafeRelease(adapterOutput);
        SafeRelease(adapter);
        SafeRelease(factory);
    }

    return refreshRate;
}

Graphics::~Graphics()
{
    SafeRelease(m_DepthStencilState);
    SafeRelease(m_RasterizerState);
    SafeRelease(m_SwapChain);
    SafeRelease(m_DeviceContext);
    SafeRelease(m_AlphaBlendState);
    SafeRelease(m_Device);

    if (m_Debug != nullptr)
        m_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    m_Debug = nullptr;
}
