#pragma once

#include "Core/Interface/ISubsystemInterface.h"
#include "Core/Interface/IFrame.h"
#include "RenderAPI/DefineRender.h"

#include "IUIElement.h"

#include <string>


class GUIManager final: public ISubsystemInterface, public IFrameInterface
{
public:
	GUIManager() = default;
	~GUIManager() override = default;

	void Initialize() override {}
	void Shutdown() override {}

	void RequestUpdate();
	void RecordingSetup() override;
	void RecordingBegin() override {}
	void RecordingEnd() override {}
	void RecordingExecute(float deltaTime) override;

	MainMenuBar& GetMainMenu();

private:

	//~ Build Layout
	void BuildScenarioGUI();
	void PopulateSceneItem(const std::string& name);
	void BuildAddObjectGUI();
	void BuildAddLightGUI();
	void BuildCameraGUI();
	void BuildColorGUI();
	void BuildSimulationGUI();

	void ShowControlPanel();

	//~ Popups
	void ShowObjectCreationPopup();
	void ShowLightCreationPopup();
	void ShowSceneCreationPopup();

	//~ Menu Windows
	void ShowCameraConfigWindow();
	void ShowColorSelectionWindow();
	void ShowSimulationWindow();

	//~ Helper Functions
	template<typename T>
	static T Max(T left, T right);

private:
	MainMenuBar			mMainMenu{};
	// Object Menu Control
	std::string				mCurrentObjectType;
	bool					mShowNewObjectPopup{ false };
	Simulation::Transform	mNewObjectTransform;
	std::string				mCurrentObjectName;
	char mFilePath[MAX_PATH]{ "" };

	// Light Menu Control
	std::string			mCurrentLightType;
	std::string			mCurrentLightName;
	Simulation::Light	mNewLightConfig;
	bool				mShowNewLightPopup{ false };

	// Scenario Menu Control
	bool				mShowScenePopup{ false };
	Menu*				mScenario{ nullptr };

	// Camera Menu Control
	bool				mShowCameraMenu{ false };

	// Color Menu Control
	float				mColor[4]{ 0.25f, 0.25f, 0.25f, 1.f };
	bool				mShowColorPicker{ false };

	// Simulation Menu Control
	bool mShowSimulationMenu{ false };
	bool mbSimulationPaused{ false };
	float mUpdatedTimeStep{ 0.0f };
};

template <typename T>
T GUIManager::Max(T left, T right)
{
	return left > right ? left : right;
}
