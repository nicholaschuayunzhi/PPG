#pragma once
#include "stdafx.h"

class Window
{
public:
    Window(
        HINSTANCE hInstance,
        int cmdShow,
        WNDPROC windowCallback,
        LPCSTR windowClassName,
        LPCSTR windowName,
        LONG windowWidth = 1280,
        LONG windowHeight = 780
    );
    HWND m_WindowHandle = 0;
    RECT GetWindowRect();
};
