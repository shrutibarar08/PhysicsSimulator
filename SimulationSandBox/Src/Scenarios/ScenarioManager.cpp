#include "Scenarios/ScenarioManager.h"
#include "RenderAPI/Shapes/DefineShape.h"

#include <iostream>
#include <fstream>


void ScenarioManager::Initialize()
{
	ISubsystemInterface::Initialize();
    // TODO: Create Simulation.ini file which will store paths
    LoadScenarios("Data/saved_scenarios.json");
}

void ScenarioManager::Shutdown()
{
	ISubsystemInterface::Shutdown();
    SaveScenarios("Data/saved_scenarios.json");
}

void ScenarioManager::CreateScenario(const std::string& name)
{
    std::string safeName = GetAvailableName(name);

    mScenarios[safeName] = std::make_unique<Scenario>();
    mNames.push_back(safeName);

    if (mActiveScenario == nullptr)
    {
        SelectScenario(safeName);
    }
}

void ScenarioManager::DeleteScenario(const std::string& name)
{
    if (mScenarios.contains(name))
    {
        UnSelectScenario(name);
        mScenarios[name].reset();
        mScenarios.erase(name);
    }
}

void ScenarioManager::SelectScenario(const std::string& name)
{
    Reset();
    if (!mScenarios.contains(name)) return;

    mActiveScenario = mScenarios[name].get();
    mScenarios[name]->OnLoad();
}

void ScenarioManager::AddObjectOnScenario(const SIMULATION_OBJECT_CREATE_DESC* desc) const
{
    if (mActiveScenario == nullptr)
    {
        std::cout << "Failed to Create Object: [No Active Scenario is Set]\n";
        return;
    }
    mActiveScenario->GetObjectManager()->AddObjectOnScene(desc);
}

void ScenarioManager::AddLightOnScenario(const SIMULATION_LIGHT_CREATE_DESC* desc) const
{
    if (mActiveScenario == nullptr)
    {
        std::cout << "Failed to Create Light: [No Active Scenario is Set]\n";
        return;
    }
    mActiveScenario->GetLightManager()->AddLight(desc);
}

void ScenarioManager::UnSelectScenario(const std::string& name)
{
    if (mScenarios[name]->IsActive())
    {
        mActiveScenario->OnUnload();
        mActiveScenario = nullptr;
    }
}

std::vector<std::string>& ScenarioManager::GetScenarioNames()
{
    return mNames;
}

void ScenarioManager::LoadScenarios(const std::string& path)
{
    std::ifstream inFile(path);
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open file for reading: " << path << std::endl;
        return;
    }

    nlohmann::json jsonData;
    inFile >> jsonData;
    inFile.close();

    for (const auto& [scenarioName, objects] : jsonData.items())
    {
        auto scenario = std::make_unique<Scenario>();
        scenario->LoadFromJson(objects);
        mScenarios[scenarioName] = std::move(scenario);
        mNames.push_back(scenarioName);
    }

    std::cout << "Scenarios loaded successfully from " << path << std::endl;
}

void ScenarioManager::SaveScenarios(const std::string& path)
{
    nlohmann::json jsonData;

    for (const auto& [scenarioName, scenario] : mScenarios)
    {
        nlohmann::json scenarioJson = scenario->SaveToJson();
        jsonData[scenarioName] = scenarioJson;
    }

    std::ofstream outFile(path);
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open file for writing: " << path << std::endl;
        return;
    }

    outFile << std::setw(4) << jsonData;  // Pretty-print with 4 spaces
    outFile.close();

    std::cout << "Scenarios saved successfully to " << path << std::endl;
}

std::string ScenarioManager::GetAvailableName(const std::string& name) const
{
    std::string safeName = name;
    if (mScenarios.contains(safeName))
    {
        int counter = 1;
        std::string newName = safeName;
        while (mScenarios.contains(newName))
            newName = safeName + "_" + std::to_string(counter++);
        safeName = newName;
    }
    return safeName;
}

void ScenarioManager::RecordingBegin()
{
}

void ScenarioManager::RecordingExecute(float deltaTime)
{
    if (mActiveScenario)
    {
        mActiveScenario->OnUpdate(deltaTime);
    }
}

Scenario* ScenarioManager::GetActiveScenario()
{
    return mActiveScenario;
}

void ScenarioManager::Reset()
{
    if (mActiveScenario != nullptr)
    {
        mActiveScenario->OnUnload();
        mActiveScenario = nullptr;
    }
}
