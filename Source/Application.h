#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Graphics.h"
#include "Input.h"

class Demo;

class Application
{
public:
    Application(HINSTANCE hInstance, int cmdShow);
    int Run(Demo& demo);
private:
    Window m_Window;
    Graphics m_Graphics; // graphics depends on window
    static LRESULT CALLBACK WindowCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void RecordInput(WPARAM wParam);
    static void ClearInput(WPARAM wParam);
    static Input input;
};
