#include "ApplicationAPI/ApplicationManager.h"
#include <windows.h>

#include <iostream>
#include <cstdio>

void EnableTerminal()
{
    // Allocate a new console if none exists
    if (AllocConsole())
    {
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);  // Redirect standard output
        freopen_s(&fp, "CONOUT$", "w", stderr);  // Redirect standard error
        freopen_s(&fp, "CONIN$", "r", stdin);    // Redirect standard input
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    EnableTerminal();
    ApplicationManager app{ L"Test Application", 1280, 720 };
    app.Execute();
    return 0;
}
