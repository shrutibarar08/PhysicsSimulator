#include "Core/Interface/ILightInterface.h"

#include <iostream>

#include "imgui/imgui.h"


void ILightInterface::InitObject(const SIMULATION_LIGHT_CREATE_DESC* desc)
{
	SetObjectName(desc->LightName);
	SetObjectType(desc->LightType);
	InitLightConfig(desc->LightData);
}

void ILightInterface::InitGUI()
{
	if (mChangeLightName.empty()) mChangeLightName.resize(150);

	// Input field
	ImGui::InputText("Set Name", mChangeLightName.data(), mChangeLightName.size());

	// Submit button
	if (ImGui::Button("Apply"))
	{
		if (mChangeLightName != mObjectName)
		{
			std::cout << "Object name changed to: " << mChangeLightName << "\n";
			SetObjectName(mChangeLightName);
		}
	}
}
