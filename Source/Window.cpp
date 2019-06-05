#include "stdafx.h"
#include "Window.h"

Window::Window(
    HINSTANCE hInstance,
    int cmdShow,
    WNDPROC windowCallback,
    LPCSTR windowClassName,
    LPCSTR windowName,
    LONG windowWidth,
    LONG windowHeight
)
{
    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = windowCallback;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = windowClassName;

    if (!RegisterClassEx(&wndClass))
    {
        throw std::exception("Window::Failed to register Class Ex");
    }

    RECT windowRect = { 0, 0, windowWidth, windowHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_WindowHandle = CreateWindowA(windowClassName, windowName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!m_WindowHandle)
    {
        throw std::exception("Window::Failed to create window");
    }

    ShowWindow(m_WindowHandle, cmdShow);
    UpdateWindow(m_WindowHandle);
}

RECT Window::GetWindowRect()
{
    RECT clientRect;
    GetClientRect(m_WindowHandle, &clientRect);
    return clientRect;
}
