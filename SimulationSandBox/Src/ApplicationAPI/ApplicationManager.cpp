#include "ApplicationAPI/ApplicationManager.h"

#include "Core/SystemManager/SubsystemManager.h"
#include "Core/SystemManager/FrameUpdateManager.h"
#include "RenderAPI/RenderQueue.h"
#include "RenderAPI/RenderManager.h"

#include "Utils/Timer.h"

#include "imgui/imgui.h"
#include "Scenarios/ScenarioManager.h"

#include "GuiAPI/GUIManager.h"
#include "WindowsAPI/WindowsManager.h"

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
    io.DisplaySize = ImVec2(
        static_cast<float>(width),
        static_cast<float>(height)
    );

    ImGui::StyleColorsDark();

    //~ Sequence Initialization is important (don't ask me why I didn't code Dependency Graph for this).
	SubsystemManager::Register<WindowsManager>(AppName, width, height);
    SubsystemManager::Register<RenderManager>();
    SubsystemManager::Register<Timer>();
    SubsystemManager::Register<RenderQueue>();
    SubsystemManager::Register<GUIManager>();
    SubsystemManager::Register<ScenarioManager>();
}

int ApplicationManager::Update()
{
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
        FrameUpdateManager::RecordExecute(SubsystemManager::Get<Timer>()->DeltaTime());
        FrameUpdateManager::RecordEnd();
    }
}
