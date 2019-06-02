#include "stdafx.h"
#include "Application.h"
#include "Demos/CubeDemo.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    Application app(hInstance, cmdShow);
    CubeDemo demo;
    app.Run(demo);
    return 0;
}
