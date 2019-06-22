#pragma once
// System includes
#include <windows.h>

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// DirectXTex
#include "DirectXTex.h"

// STL includes
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr)
{
    if (ptr != NULL)
    {
        ptr->Release();
        ptr = NULL;
    }
}


inline DirectX::XMFLOAT3 XMFLOAT3Subtract(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
    return { a.x - b.x,
             a.y - b.y,
             a.z - b.z };
}

inline DirectX::XMFLOAT2 XMFLOAT2Subtract(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
{
    return { a.x - b.x,
             a.y - b.y };
}
