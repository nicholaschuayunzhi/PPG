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

    HWND hDesktop = GetDesktopWindow();
    RECT desktop;
    GetWindowRect(hDesktop, &desktop);
    long prevMouseX = 0, prevMouseY = 0;

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

            POINT cursorPos;
            GetCursorPos(&cursorPos);

            if (input.mouse & Mouse::RMB_DOWN)
            {
                input.deltaMouseX = (cursorPos.x - prevMouseX) / 500.0f;
                input.deltaMouseY = (cursorPos.y - prevMouseY) / 500.0f;
            }

            m_Graphics.Clear(Colors::CornflowerBlue, 1.0f, 0);
            demo.Update(m_Graphics, input, deltaTime);
            m_Graphics.Present();

            prevMouseX = cursorPos.x;
            prevMouseY = cursorPos.y;
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
            hDC = BeginPaint(hwnd, &paintStruct);
            EndPaint(hwnd, &paintStruct);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
                break;
            }
        case WM_RBUTTONDOWN:
            RecordInput(wParam);
            break;
        case WM_KEYUP:
            ClearInput(wParam);
            break;
        case WM_RBUTTONUP:
            ClearInput(VK_RBUTTON);
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
            input.key &= ~Key::FRONT;
            break;
        case 'S':
            input.key &= ~Key::BACK;
            break;
        case 'A':
            input.key &= ~Key::LEFT;
            break;
        case 'D':
            input.key &= ~Key::RIGHT;
            break;
        case 'Q':
            input.key &= ~Key::UP;
            break;
        case 'E':
            input.key &= ~Key::DOWN;
            break;
        case VK_RBUTTON:
            input.mouse &= ~Mouse::RMB_DOWN;
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
            input.key |= Key::FRONT;
            break;
        case 'S':
            input.key |= Key::BACK;
            break;
        case 'A':
            input.key |= Key::LEFT;
            break;
        case 'D':
            input.key |= Key::RIGHT;
            break;
        case 'Q':
            input.key |= Key::UP;
            break;
        case 'E':
            input.key |= Key::DOWN;
            break;
        case VK_RBUTTON:
            input.mouse |= Mouse::RMB_DOWN;
            break;
        default:
            return;
    }
    return;
}
