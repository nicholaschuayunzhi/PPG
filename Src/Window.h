#pragma once
#include "stdafx.h"

class Window
{
public:
    Window(
        HINSTANCE hInstance,
        int cmdShow,
        LPCSTR windowClassName,
        LPCSTR windowName,
        LONG windowWidth = 1280,
        LONG windowHeight = 780
    );
    HWND m_WindowHandle = 0;
    RECT GetWindowRect();
private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
