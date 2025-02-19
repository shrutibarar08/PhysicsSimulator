#include "WindowSystem.h"
#include "RenderAPI.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    try
    {
        UNREFERENCED_PARAMETER(hPrevInstance);
        WindowSystem::Init(hInstance, 1270, 720,L"Simulation Sand Box");
        RenderAPI::Init();

        while (true)
        {
            if (std::optional<int> code = WindowSystem::ProcessMessage(); code.has_value())
            {
                return code.value();
            }
            RenderAPI::Render();
        }
    }
    catch (std::exception& error)
    {
        std::string msg = error.what();
        std::wstring errorMsg = std::wstring(msg.begin(), msg.end());
        MessageBox(nullptr, errorMsg.c_str(), L"Error Found", MB_ICONERROR);

    	return E_FAIL;
    }
}
