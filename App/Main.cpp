#include "../Engine/Source/PPG.h"
#include "MyDemo.h"
#include "PBRSpheres.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    Application app(hInstance, cmdShow);
    PBRSpheres demo;
    app.Run(demo);
    return 0;
}
