#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Graphics.h"

class Demo;
class Window;
class Graphics;

class Application
{
public:
    Application(HINSTANCE hInstance, int cmdShow);
    int Run(Demo& demo);
private:
    Window m_Window;
    Graphics m_Graphics; // graphics depends on window
};
