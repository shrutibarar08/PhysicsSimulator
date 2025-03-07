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
	void ShowControlPanel();

	//~ Popups
	void ShowObjectCreationPopup();
	void ShowLightCreationPopup();
	void ShowSceneCreationPopup();
	void ShowCameraConfigWindow();
	void ShowColorSelectionWindow();

	//~ Helper Functions
	template<typename T>
	static T Max(T left, T right);

private:
	MainMenuBar			mMainMenu{};
	std::string			mCurrentObjectType;
	std::string			mCurrentObjectName;
	char mFilePath[MAX_PATH] = "";
	std::string			mCurrentLightType;
	std::string			mCurrentLightName;
	Simulation::Transform	mNewObjectTransform;
	Simulation::Light		mNewLightConfig;
	bool				mShowNewObjectPopup{ false };
	bool				mShowNewLightPopup{ false };
	bool				mShowScenePopup{ false };
	bool				mShowCameraMenu{ false };
	Menu*				mScenario{ nullptr };
	float				mColor[4]{ 0.25f, 0.25f, 0.25f, 1.f };
	bool				mShowColorPicker{ false };
};

template <typename T>
T GUIManager::Max(T left, T right)
{
	return left > right ? left : right;
}
