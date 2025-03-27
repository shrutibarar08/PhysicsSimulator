#include "GuiAPI/PhysicsUI/ParticleSystemUI.h"

#include <iostream>

#include "Core/RegistryManager/RegistryEffect.h"
#include "imgui/imgui.h"


void ParticleSystemUI::OnUpdateGUI()
{
    CreationOptionUI();
    ImGui::Separator();

    if (mParticleEffects.empty())
    {
        ImGui::Text("No effects added.");
        return;
    }

    for (size_t i = 0; i < mParticleEffects.size(); ++i)
    {
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("%s", mParticleEffects[i].name.c_str());

        // Remove button
        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            std::cout << "Removing effect: " << mParticleEffects[i].name << "\n";
            mParticleSystem.RemoveEffect(mParticleEffects[i].id);
            mParticleEffects.erase(mParticleEffects.begin() + i);
        }
        ImGui::PopID();
    }
}

void ParticleSystemUI::LoadFromJson(const nlohmann::json& jsonData)
{
}

nlohmann::json ParticleSystemUI::SaveToJson()
{
	return nlohmann::json();
}

ParticleSystem* ParticleSystemUI::GetParticleSystem()
{
	return &mParticleSystem;
}

void ParticleSystemUI::CreationOptionUI()
{
    if (ImGui::Button("Add Particle Effect"))
    {
        ImGui::OpenPopup("Particle Effect Selector");
    }
    ImGui::SameLine();
    if (ImGui::BeginPopupModal("Particle Effect Selector", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Select an Effect Type:");

        // Get effect names from RegistryEffect
        const auto& effectNames = RegistryEffect::GetRegisteredNames();

        // Ensure selected effect index is valid
        const char* selectedEffectName =
            (mEffectIndex >= 0 && mEffectIndex < effectNames.size()) ?
            effectNames[mEffectIndex].c_str() : "None";

        // Dropdown menu
        if (ImGui::BeginCombo("##EffectSelector", selectedEffectName))
        {
            for (int i = 0; i < effectNames.size(); i++)
            {
                bool isSelected = (mEffectIndex == i);
                if (ImGui::Selectable(effectNames[i].c_str(), isSelected))
                {
                    mEffectIndex = i;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // OK button confirms selection
        if (ImGui::Button("OK") && mEffectIndex >= 0 && mEffectIndex < effectNames.size())
        {
            std::cout << "Selected Effect: " << effectNames[mEffectIndex] << "\n";

            auto effect = RegistryEffect::Create(effectNames[mEffectIndex]);
            if (!effect)
            {
                std::cerr << "Failed to create effect: " << effectNames[mEffectIndex] << std::endl;
                return;
            }

            int id = mParticleSystem.AddEffect(effect.get());
            mParticleEffects.emplace_back(id, effectNames[mEffectIndex], std::move(effect));

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        // Cancel button closes the popup
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
