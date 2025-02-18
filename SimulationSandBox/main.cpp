#include "WindowSystem.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    WindowSystem::Init(hInstance, 800, 600, "Sandbox");
    WindowSystem* window = WindowSystem::Get();

    while (window->ProcessMessage()){
    }

    return 0;
}
