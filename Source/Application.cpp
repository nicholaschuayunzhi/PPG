#include "stdafx.h"
#include "Application.h"
#include "Demos/Demo.h"
#include "Graphics.h"
#include "Window.h"
#include "Input.h"

Input Application::input;

Application::Application(HINSTANCE hInstance, int cmdShow) :
    m_Window(hInstance, cmdShow, &WindowCallback, "DirectXWindowClass", "PPG"),
    m_Graphics(hInstance, true, m_Window)
{
    if (!DirectX::XMVerifyCPUSupport())
    {
        throw std::exception("Application::Failed to verify DirectX Math library support.");
    }
}

int Application::Run(Demo& demo)
{
    m_Graphics.SetUp();
    demo.Start(m_Graphics);
    MSG msg = { 0 };
    static DWORD previousTime = timeGetTime();
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            DWORD currentTime = timeGetTime();
            float deltaTime = (currentTime - previousTime) / 1000.0f;
            previousTime = currentTime;
            m_Graphics.Clear(Colors::CornflowerBlue, 1.0f, 0);
            demo.Update(m_Graphics, input, deltaTime);
            m_Graphics.Present();
        }
    }
    demo.End();
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Application::WindowCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paintStruct;
    HDC hDC;

    switch (message)
    {
        case WM_PAINT:
            {
                hDC = BeginPaint(hwnd, &paintStruct);
                EndPaint(hwnd, &paintStruct);
            }
            break;
        case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            break;
        case WM_KEYDOWN:
            RecordInput(wParam);
            break;
        case WM_KEYUP:
            ClearInput(wParam);
            break;
        default:
           return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

void Application::ClearInput(WPARAM wParam)
{
    switch (wParam)
    {
        case 'W':
        case VK_UP:
            input &= ~Key::UP;
            break;
        case 'S':
        case VK_DOWN:
            input &= ~Key::DOWN;
            break;
        case 'A':
        case VK_LEFT:
            input &= ~Key::LEFT;
            break;
        case 'D':
        case VK_RIGHT:
            input &= ~Key::RIGHT;
            break;
        default:
            return;
        }
    return;
}

void Application::RecordInput(WPARAM wParam)
{
    switch (wParam)
    {
        case 'W':
        case VK_UP:
           input |= Key::UP;
           break;
        case 'S':
        case VK_DOWN:
           input |= Key::DOWN;
           break;
        case 'A':
        case VK_LEFT:
           input |= Key::LEFT;
           break;
        case 'D':
        case VK_RIGHT:
            input |= Key::RIGHT;
            break;
        default:
            return;
    }
    return;
}
