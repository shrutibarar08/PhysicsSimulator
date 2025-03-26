#include <iostream>

#include "Core/Components/PhysicsObject.h"
#include "imgui/imgui.h"

#include "PhysicsAPI/DefineEffects.h"
#include "PhysicsAPI/DefineCollider.h"


void PhysicsObject::InitParticleEffectPopUp()
{

    if (ImGui::Button("Add Particle Effect"))
    {
        ImGui::OpenPopup("Particle Effect Selector");
    }

    if (ImGui::BeginPopupModal("Particle Effect Selector", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Select an Effect Type:");

        // Get effect names from RegistryEffect
        const auto& effectNames = RegistryEffect::GetRegisteredNames();

        // Ensure selected effect index is valid
        const char* selectedEffectName =
            (mColliderIndex >= 0 && mColliderIndex < effectNames.size()) ?
            effectNames[mColliderIndex].c_str() : "None";

        // Dropdown menu
        if (ImGui::BeginCombo("##EffectSelector", selectedEffectName))
        {
            for (int i = 0; i < effectNames.size(); i++)
            {
                bool isSelected = (mColliderIndex == i);
                if (ImGui::Selectable(effectNames[i].c_str(), isSelected))
                {
                    mColliderIndex = i;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // OK button confirms selection
        if (ImGui::Button("OK") && mColliderIndex >= 0 && mColliderIndex < effectNames.size())
        {
            std::cout << "Selected Effect: " << effectNames[mColliderIndex] << "\n";

            auto effect = RegistryEffect::Create(effectNames[mColliderIndex]);
            if (!effect)
            {
                std::cerr << "Failed to create effect: " << effectNames[mColliderIndex] << std::endl;
                return;
            }

            int id = mParticleSystem.AddEffect(effect.get());
            mParticleEffects.emplace_back(id, effectNames[mColliderIndex], std::move(effect));

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

void PhysicsObject::InitColliderPopUp()
{
    ImGui::Separator();
    if (ImGui::Button("Add Collider"))
    {
        ImGui::OpenPopup("Collider Selector");
    }

    if (ImGui::BeginPopupModal("Collider Selector", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Select Collider Type:");

        // Get effect names from RegistryEffect
        const auto& colliderNames = RegistryCollider::GetRegisteredNames();

        // Ensure selected effect index is valid
        const char* selectedColliderName =
            (mColliderIndex >= 0 && mColliderIndex < colliderNames.size()) ?
            colliderNames[mColliderIndex].c_str() : "None";

        // Dropdown menu
        if (ImGui::BeginCombo("##ColliderSelector", selectedColliderName))
        {
            for (int i = 0; i < colliderNames.size(); i++)
            {
                bool isSelected = (mColliderIndex == i);
                if (ImGui::Selectable(colliderNames[i].c_str(), isSelected))
                {
                    mColliderIndex = i;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // OK button confirms selection
        if (ImGui::Button("OK") && mColliderIndex >= 0 && mColliderIndex < colliderNames.size())
        {
            std::cout << "Selected Effect: " << colliderNames[mColliderIndex] << "\n";

            auto effect = RegistryCollider::Create(colliderNames[mColliderIndex]);
            if (!effect)
            {
                std::cerr << "Failed to create Collider: " << colliderNames[mColliderIndex] << std::endl;
                return;
            }

            mCollider.reset();
            mCollider = std::move(effect);
            mCollider->AttachParticle(&mParticle);

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

void PhysicsObject::InitColliderUpdateGUI()
{
    ImGui::Text("Attached Collider:");
    if (mCollider == nullptr)
    {
        ImGui::Text("No Collider Attached");
        return;
    }
    ImGui::Text("%s", mCollider->GetColliderName().c_str());
    ImGui::SameLine();
    if (ImGui::Button("Remove"))
    {
        std::cout << "Collider Removed!\n";
        mCollider.reset();
    }

    ImGui::Text("Properties:");
    ImGui::DragFloat("Elastic", &mCollider->Elastic, 0.1f,
        0.0f, 1.0f, "%0.002f");

    ImGui::Checkbox("Static", &mCollider->bStatic);

    ImGui::Separator();
}

void PhysicsObject::InitParticleUpdateGUI()
{
    ImGui::Text("Active Particle Effects:");

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

void PhysicsObject::Update(float deltaTime)
{
    if (mbEffects) mParticleSystem.Update(mParticle, deltaTime);
}

void PhysicsObject::ToggleEffects()
{
    mbEffects = !mbEffects;
}
