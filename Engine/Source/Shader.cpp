#include "stdafx.h"
#include "Shader.h"
#include "Graphics.h"

Shader::Shader(LPCWSTR vertexPath, LPCWSTR pixelPath, Graphics& graphics)
{
    ID3DBlob* vertexShaderBlob;
    HRESULT hr = D3DReadFileToBlob(vertexPath, &vertexShaderBlob);

    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create vertex shader blob");
    }

    hr = graphics.m_Device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &m_VertexShader);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create vertex shader");
    }

    // Currently only support this type
    D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = graphics.m_Device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_InputLayout);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create input layout");
    }
    SafeRelease(vertexShaderBlob);

    ID3DBlob* pixelShaderBlob;
    LPCWSTR compiledPixelShaderObject = pixelPath;

    hr = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create pixel shader blob");
    }

    hr = graphics.m_Device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &m_PixelShader);
    if (FAILED(hr))
    {
        throw std::exception("Graphics::Failed to create pixel shader");
    }

    SafeRelease(pixelShaderBlob);
}

Shader::~Shader()
{
    SafeRelease(m_VertexShader);
    SafeRelease(m_PixelShader);
}

void Shader::Use(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout(m_InputLayout);
    deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
    deviceContext->PSSetShader(m_PixelShader, nullptr, 0);
}
