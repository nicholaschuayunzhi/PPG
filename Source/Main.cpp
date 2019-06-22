#include "stdafx.h"
#include "Application.h"
#include "Demos/NormalMapDemo.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    Application app(hInstance, cmdShow);
    NormalMapDemo demo;
    app.Run(demo);
    return 0;
}
