#include "WindowSystem.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    WindowSystem::Init(hInstance, 800, 600, "Sandbox");
    WindowSystem* window = WindowSystem::Get();

    //Initialize DirectX 11
    window->InitDirectX();

    while (window->ProcessMessage()) {
        // Game loop or rendering logic
        float clearColor[] = { 0.1f, 0.1f, 0.2f, 1.0f }; // RGBA
        window->GetDeviceContext()->ClearRenderTargetView(
            window->GetRenderTargetView(), clearColor
        );
		window->GetSwapChain()->Present(1, 0);
    }
    return 0;
}
