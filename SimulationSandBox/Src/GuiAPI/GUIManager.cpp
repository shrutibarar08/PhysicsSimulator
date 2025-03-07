#include "GuiAPI/GUIManager.h"

#include "RenderAPI/Shapes/DefineShape.h"
#include "RenderAPI/Light/DefineLight.h"

#include "Core/SystemManager/SubsystemManager.h"
#include "RenderAPI/RenderManager.h"
#include "Scenarios/ScenarioManager.h"

#include "WindowsAPI/WindowsHelper.h"

#include <algorithm>

void GUIManager::RequestUpdate()
{
	BuildScenarioGUI();
	BuildAddObjectGUI();
	BuildAddLightGUI();
	BuildCameraGUI();
	BuildColorGUI();
}

void GUIManager::RecordingSetup()
{
	IFrameInterface::RecordingSetup();
	RequestUpdate();
}

void GUIManager::RecordingExecute(float deltaTime)
{
	IFrameInterface::RecordingExecute(deltaTime);
	mMainMenu.Render();
	ShowControlPanel();
	ShowObjectCreationPopup();
	ShowLightCreationPopup();
	ShowSceneCreationPopup();
	ShowCameraConfigWindow();
	ShowColorSelectionWindow();
}

MainMenuBar& GUIManager::GetMainMenu()
{
	return mMainMenu;
}

void GUIManager::BuildScenarioGUI()
{
	auto sceneMenu = std::make_unique<Menu>("Scenario");
	mScenario = sceneMenu.get();

	auto sceneCreationItem = std::make_unique<MenuItem>("Create Scenario",
		[&]()
		{
			mShowScenePopup = true;
		});

	sceneMenu->AddItem(std::move(sceneCreationItem));

	for (auto& name : SubsystemManager::Get<ScenarioManager>()->GetScenarioNames())
	{
		PopulateSceneItem(name);
	}

	mMainMenu.AddMenu(std::move(sceneMenu));
}

void GUIManager::PopulateSceneItem(const std::string& name)
{
	auto subMenu = std::make_unique<Menu>(name);

	auto select = std::make_unique<MenuItem>(
		"Select",
		[name]()
		{
			SubsystemManager::Get<ScenarioManager>()->SelectScenario(name);
		}
	);

	auto unSelect = std::make_unique<MenuItem>(
		"UnSelect",
		[name]()
		{
			SubsystemManager::Get<ScenarioManager>()->UnSelectScenario(name);
		}
	);

	subMenu->AddItem(std::move(select));
	subMenu->AddItem(std::move(unSelect));

	mScenario->AddItem(std::move(subMenu));
}

void GUIManager::BuildCameraGUI()
{
	auto cameraMenuItem = std::make_unique<MenuItem>(
		"Camera",
		[&]()
		{ mShowCameraMenu = !mShowCameraMenu; }  // Toggle visibility
	);

	mMainMenu.AddMenu(std::move(cameraMenuItem));
}

void GUIManager::BuildColorGUI()
{
	auto colorItem = std::make_unique<MenuItem>("Color",
		[&]()
		{
			mShowColorPicker = !mShowColorPicker; // Toggle the color picker window
		});
	mMainMenu.AddMenu(std::move(colorItem));
}

void GUIManager::ShowControlPanel()
{
	if (Scenario* scene = SubsystemManager::Get<ScenarioManager>()->GetActiveScenario())
	{
		static bool firstRun = true;
		static ImVec2 lastWindowPos = { 50, 50 }; // Starting position

		if (firstRun)
		{
			ImGui::SetNextWindowPos(lastWindowPos);
			firstRun = false;
		}

		// First Window - Object Manager
		ImGui::Begin("Object Manager");
		scene->GetObjectManager()->InitUpdateGUI();
		lastWindowPos = ImGui::GetWindowPos();
		lastWindowPos.y += ImGui::GetWindowSize().y + 5; // Stack next window below with spacing
		ImGui::End();

		// Second Window - Light Manager
		ImGui::SetNextWindowPos(lastWindowPos);
		ImGui::Begin("Light Manager");
		scene->GetLightManager()->InitUpdateGUI();
		lastWindowPos = ImGui::GetWindowPos();
		lastWindowPos.y += ImGui::GetWindowSize().y + 5;
		ImGui::End();
	}
}

void GUIManager::BuildAddObjectGUI()
{
	auto objMenu = std::make_unique<Menu>("Add Object");

	for (auto& name : RegistryShape::GetRegisteredNames())
	{
		auto item = std::make_unique<MenuItem>(
			name,
			[this, name]()
			{
				// Set the object name and reset transformation values
				mCurrentObjectType = name;
				mNewObjectTransform = Simulation::Transform();
				mShowNewObjectPopup = true;
				mCurrentObjectName = "";
			}
		);
		objMenu->AddItem(std::move(item));
	}

	mMainMenu.AddMenu(std::move(objMenu));
}

void GUIManager::BuildAddLightGUI()
{
	auto lightMenu = std::make_unique<Menu>("Add Light");

	for (auto& name : RegistryLight::GetRegisteredNames())
	{
		auto item = std::make_unique<MenuItem>(
			name,
			[this, name]()
			{
				// Set the object name and reset transformation values
				mCurrentLightType = name;
				mNewLightConfig = Simulation::Light();
				mShowNewLightPopup = true;
				mCurrentLightName = "";
			}
		);
		lightMenu->AddItem(std::move(item));
	}

	mMainMenu.AddMenu(std::move(lightMenu));
}

void GUIManager::ShowObjectCreationPopup()
{
	if (mCurrentObjectName.empty()) mCurrentObjectName.resize(128);

	if (mShowNewObjectPopup)
	{
		ImGui::OpenPopup("Create Object");
		mShowNewObjectPopup = false;
	}

	if (ImGui::BeginPopupModal("Create Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Set Object Name", mCurrentObjectName.data(), mCurrentObjectName.size());

		ImGui::Text("Set Transform Values for %s", mCurrentObjectType.c_str());
		// Editable fields for position, rotation, and scale
		ImGui::InputFloat3("Translation", &mNewObjectTransform.Translation.x);
		ImGui::InputFloat3("Rotation", &mNewObjectTransform.Rotation.x);
		ImGui::InputFloat3("Scale", &mNewObjectTransform.Scale.x);

		ImGui::Separator();

		// File path input (Read-Only)
		ImGui::InputText("File Path", mFilePath, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		// "Browse" button
		if (ImGui::Button("Browse"))
		{
			std::string selectedFile = OpenFileDialog();
			if (!selectedFile.empty())
			{
				strcpy_s(mFilePath, selectedFile.c_str());
			}
		}

		ImGui::Separator();

		// "Create" button
		if (ImGui::Button("Create"))
		{
			// Send object data to the ScenarioManager
			SIMULATION_OBJECT_CREATE_DESC desc{};
			desc.Transform = mNewObjectTransform;
			desc.ObjectName = mCurrentObjectName;
			desc.ObjectType = mCurrentObjectType;
			desc.TexturePath = mFilePath;
;
			SubsystemManager::Get<ScenarioManager>()->AddObjectOnScenario(&desc);
			ImGui::CloseCurrentPopup(); // Close popup after creation
		}

		ImGui::SameLine();

		// "Cancel" button
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup(); // Close without saving
		}
		ImGui::EndPopup();
	}
}

void GUIManager::ShowLightCreationPopup()
{
	if (mCurrentLightName.empty()) mCurrentLightName.reserve(125);
	if (mShowNewLightPopup)
	{
		ImGui::OpenPopup("Create Light");
		mShowNewLightPopup = false;
	}

	if (ImGui::BeginPopupModal("Create Light", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Set Light Name", mCurrentLightName.data(), mCurrentObjectName.size());

		ImGui::Text("Set Light Properties");

		ImGui::ColorEdit4("Ambient", &mNewLightConfig.Ambient.x);
		ImGui::ColorEdit4("Diffuse", &mNewLightConfig.Diffuse.x);
		ImGui::ColorEdit4("Specular", &mNewLightConfig.Specular.x);
		ImGui::InputFloat3("Position", &mNewLightConfig.Position.x);
		ImGui::InputFloat("Range", &mNewLightConfig.Range);
		ImGui::InputFloat3("Direction", &mNewLightConfig.Direction.x);
		ImGui::InputFloat("Spot", &mNewLightConfig.Spot);
		ImGui::InputFloat3("Attenuation", &mNewLightConfig.Att.x);

		ImGui::Separator();

		if (ImGui::Button("Create"))
		{
			SIMULATION_LIGHT_CREATE_DESC desc{};
			desc.LightType = mCurrentLightType;
			desc.LightName = mCurrentLightName;
			desc.LightData = mNewLightConfig;
			SubsystemManager::Get<ScenarioManager>()->AddLightOnScenario(&desc);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void GUIManager::ShowSceneCreationPopup()
{
	if (mShowScenePopup)
	{
		ImGui::OpenPopup("Create Scene");
		mShowScenePopup = false;
	}

	if (ImGui::BeginPopupModal("Create Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter Scene Name:");

		// Editable field for scene name
		static char sceneName[128] = "";  // Holds input text
		ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName));

		ImGui::Separator();

		// "Create" button
		if (ImGui::Button("Create") && sceneName[0] != '\0') // Ensure name isn't empty
		{
			SubsystemManager::Get<ScenarioManager>()->CreateScenario(sceneName);
			PopulateSceneItem(sceneName);
			ImGui::CloseCurrentPopup(); // Close after creation
		}

		ImGui::SameLine();

		// "Cancel" button
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup(); // Close without saving
		}

		ImGui::EndPopup();
	}
}

void GUIManager::ShowCameraConfigWindow()
{
	if (!mShowCameraMenu || !SubsystemManager::Get<ScenarioManager>()->GetActiveScenario()) return;

	// Set a bigger window size
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Camera Settings", &mShowCameraMenu,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	CameraComponent& activeCamera =
		SubsystemManager::Get<ScenarioManager>()->GetActiveScenario()->GetCameraComponent();

	static float fov = activeCamera.GetFieldOfView();
	static float farClip = activeCamera.GetMaxVisibleDistance();
	static DirectX::XMFLOAT3 cameraPosition = { 0.0f, 0.0f, 0.0f };
	static DirectX::XMFLOAT3 cameraRotation = { 0.0f, 0.0f, 0.0f };
	static bool holdMode = true; // Hold mode for continuous movement

	// Set a larger font scale
	ImGui::SetWindowFontScale(1.5f);

	// Increase spacing
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

	// Camera Position
	ImGui::Text("Position");
	for (int i = 0; i < 3; i++)
	{
		ImGui::SameLine();
		if (ImGui::Button(("-##Pos" + std::to_string(i)).c_str(), ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive()))
			activeCamera.AddTranslation(i, -0.02f);

		ImGui::SameLine();
		ImGui::Text("%c", 'X' + i);

		ImGui::SameLine();
		if (ImGui::Button(("+##Pos" + std::to_string(i)).c_str(), ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive()))
			activeCamera.AddTranslation(i, 0.02f);
	}

	// Camera Rotation
	ImGui::Text("Rotation");
	for (int i = 0; i < 3; i++)
	{
		ImGui::SameLine();
		if (ImGui::Button(("-##Rot" + std::to_string(i)).c_str(), ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive()))
			activeCamera.Rotate(i, -0.002f);

		ImGui::SameLine();
		ImGui::Text("%c", 'X' + i);

		ImGui::SameLine();
		if (ImGui::Button(("+##Rot" + std::to_string(i)).c_str(), ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive()))
			activeCamera.Rotate(i, 0.002f);
	}

	// FOV
	ImGui::Text("Field of View");
	ImGui::SameLine();
	if (ImGui::Button("-##FOV", ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive())) activeCamera.SetFieldOfView(fov -= .002f);
	ImGui::SameLine();
	ImGui::Text("%.01f", fov);
	ImGui::SameLine();
	if (ImGui::Button("+##FOV", ImVec2(40, 40)) || (holdMode && ImGui::IsItemActive())) activeCamera.SetFieldOfView(fov += 0.002f);

	static int projectionMode = (activeCamera.GetCameraMode() == Simulation::CameraState::Perspective) ? 0 : 1;

	if (ImGui::RadioButton("Perspective", projectionMode == 0))
	{
		projectionMode = 0;
		activeCamera.SetCameraMode(Simulation::CameraState::Perspective);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Orthographic", projectionMode == 1))
	{
		projectionMode = 1;
		activeCamera.SetCameraMode(Simulation::CameraState::Orthographic);
	}

	// Close Button
	if (ImGui::Button("Close", ImVec2(100, 50)))
	{
		mShowCameraMenu = false;
	}

	ImGui::PopStyleVar();
	ImGui::End();
}

void GUIManager::ShowColorSelectionWindow()
{
	if (!mShowColorPicker) return; // Only show if toggled on

	ImGui::Begin("Color Picker", &mShowColorPicker); // The second argument adds a close button

	if (ImGui::ColorEdit4("Color", mColor))
	{
		SubsystemManager::Get<RenderManager>()->SetBackgroundColor(mColor);
	}

	ImGui::End();
}
