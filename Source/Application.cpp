#include "stdafx.h"
#include "Application.h"
#include "Demos/Demo.h"
#include "Graphics.h"
#include "Window.h"

Application::Application(HINSTANCE hInstance, int cmdShow) :
    m_Window(hInstance, cmdShow, "DirectXWindowClass", "PPG"),
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

            m_Graphics.Clear(Colors::BlanchedAlmond, 1.0f, 0);
            demo.Update(m_Graphics, deltaTime);
            m_Graphics.Present();
        }
    }
    demo.End();
    return static_cast<int>(msg.wParam);
}
