#pragma once

#include "Core/Interface/ISubsystemInterface.h"
#include "Core/Interface/IFrame.h"
#include "Scenarios/Scenario.h"

#include "nlohmann/json.hpp"
#include <unordered_map>

/*
 *	Manages Scenarios
 */
class ScenarioManager final: public ISubsystemInterface, public IFrameInterface
{
public:
	~ScenarioManager() override = default;
	void Initialize() override;
	void Shutdown() override;

	void CreateScenario(const std::string& name);
	void DeleteScenario(const std::string& name);
	void SelectScenario(const std::string& name);

	void AddObjectOnScenario(const SIMULATION_OBJECT_CREATE_DESC* desc) const;
	void AddLightOnScenario(const SIMULATION_LIGHT_CREATE_DESC* desc) const;

	//~ Getters and Setter
	void UnSelectScenario(const std::string& name);
	std::vector<std::string>& GetScenarioNames();

	void RecordingBegin() override;
	void RecordingExecute(float deltaTime) override;

	Scenario* GetActiveScenario();

private:
	void LoadScenarios(const std::string& path);
	void SaveScenarios(const std::string& path);

	void Reset();

	std::string GetAvailableName(const std::string& name) const;

private:
	std::unordered_map<std::string, std::unique_ptr<Scenario>> mScenarios;
	std::vector<std::string> mNames;

	Scenario* mActiveScenario{ nullptr };
};
