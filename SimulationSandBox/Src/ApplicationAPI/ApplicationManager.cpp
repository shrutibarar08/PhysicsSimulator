#include "ApplicationAPI/ApplicationManager.h"

#include "Core/SubsystemManager.h"
#include "Core/FrameUpdateManager.h"
#include "RenderAPI/RenderQueue.h"
#include "RenderAPI/RenderManager.h"

#include "Utils/Timer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#ifdef _DEBUG
#include <iostream>
#endif

ApplicationManager::ApplicationManager(const std::wstring& AppName, int width, int height)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 20.0f); // Increase font size
    io.Fonts->Build();

    ImGui::StyleColorsDark();

    //~ Sequence Initialization is important (don't ask me why I didn't code Dependency Graph for this).
	SubsystemManager::Register<WindowsManager, false>(AppName, width, height);
    SubsystemManager::Register<RenderManager, false>();
    SubsystemManager::Register<Timer, false>();
    SubsystemManager::Register<RenderQueue, false>();
}

void ApplicationManager::BeginPlay()
{

}

void ApplicationManager::Tick()
{
    if (ImGui::BeginMainMenuBar())  // Start the menu bar
    {
        if (ImGui::BeginMenu("Scenario"))
        {
            if (ImGui::BeginMenu("Plane 1")) // Submenu for Open
            {
                if (ImGui::MenuItem("Load")) {
                    planeScenario_1.OnLoad();
                }
                if (ImGui::MenuItem("UnLoad")) {
                    planeScenario_1.OnUnload();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Plane 2")) // Submenu for Open
            {
                if (ImGui::MenuItem("Load")) {
                    planeScenario_2.OnLoad();
                }
                if (ImGui::MenuItem("UnLoad")) {
                    planeScenario_2.OnUnload();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int ApplicationManager::Update()
{
    BeginPlay();
    FrameUpdateManager::RecordSetup();
    while (true)
    {
        auto message = WindowsManager::ProcessMessages();
        if (message.has_value())
        {
            SubsystemManager::ShutdownAll();
            return message.value();
        }
        FrameUpdateManager::RecordStart();
        FrameUpdateManager::RecordExecute();
        Tick();
        FrameUpdateManager::RecordEnd();
    }
}
