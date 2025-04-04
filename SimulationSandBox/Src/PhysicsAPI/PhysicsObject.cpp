#include <iostream>

#include "PhysicsAPI/PhysicsObject.h"

#include "GuiAPI/IUIElement.h"
#include "imgui/imgui.h"

#include "PhysicsAPI/DefineEffects.h"
#include "PhysicsAPI/DefineCollider.h"


void PhysicsObject::InitParticleEffectsUpdateGUI()
{
    SubOptionElement::DrawSubOption(
        "Active Effects",
        [&]()
        {
            mParticleSystem.OnUpdateGUI();
        }
    );
}

void PhysicsObject::InitGUI()
{
    InitParticleUpdateGUI();
    InitParticleEffectsUpdateGUI();
    InitColliderUpdateGUI();
}

void PhysicsObject::InitColliderPopUp()
{
    if (mCollider != nullptr) return;

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
            mCollider->Collider()->AttachParticle(mParticle.GetParticle());
            mCollider->Collider()->SetColliderName(colliderNames[mColliderIndex]);

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
    SubOptionElement::DrawSubOption("Collider",
        [&]()
        {
            InitColliderPopUp();

            if (mCollider == nullptr) return;
            mCollider->OnUpdateGUI();

            if (ImGui::Button("Remove Collider"))
            {
                mCollider.reset();
            }
        });
}

void PhysicsObject::Update(float deltaTime)
{
    if (mCollider != nullptr) mCollider->Update(deltaTime);
    if (mbEffects) mParticleSystem.GetParticleSystem()->Update(mParticle.GetParticle(), deltaTime);
}

void PhysicsObject::ToggleEffects()
{
    mbEffects = !mbEffects;
}

nlohmann::json PhysicsObject::SaveToJson()
{
    nlohmann::json cache;

    cache["Particle"] = mParticle.SaveToJson();
    if (mCollider != nullptr)
        cache["Collider"] = mCollider->SaveToJson();
    else cache["Collider"] = {};

    return cache;
}

void PhysicsObject::LoadFromJson(const nlohmann::json& json)
{
    if (json.contains("Particle") && json["Particle"].is_object())
        mParticle.LoadFromJson(json["Particle"]);

    if (json.contains("Collider") && json["Collider"].is_object())
    {
        mCollider = RegistryCollider::Create(json["Collider"]["Name"]);
        mCollider->Collider()->AttachParticle(mParticle.GetParticle());
        mCollider->Collider()->SetColliderName(json["Collider"]["Name"]);
        mCollider->LoadFromJson(json["Collider"]);
    }
}

void PhysicsObject::InitParticleUpdateGUI()
{
    SubOptionElement::DrawSubOption("Particle",
        [&]()
        {
            mParticle.OnUpdateGUI();
        });
}
