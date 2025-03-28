#include "GuiAPI/PhysicsUI/ColliderGUI/IColliderUI.h"

#include "GuiAPI/IUIElement.h"
#include "imgui/imgui.h"


void IColliderUI::Update(float deltaTime)
{
    Collider()->UpdateRestingStates(deltaTime);
}

void IColliderUI::OnUpdateGUI()
{
    ChildUpdateGUI();

    ImGui::SliderInt("Rest Threshold",
        &Collider()->mRestThreshold, 1, 50);
    ImGui::SliderFloat("Rest Time Threshold",
        &Collider()->mRestTimeThreshold, 0.1f,
        5.0f, "%.2f sec");

    SubOptionElement::DrawSubOption(
        "Collided Object",
        [&]()
        {
            for (auto& [collider, info] : Collider()->mCollisionMap)
            {
                ImGui::PushID(collider); // Ensure unique ID for each entry

                ImGui::Text("Collider: %s", collider->GetColliderName().c_str());
                ImGui::Text("  Hit Count: %d", info.hitCount);
                ImGui::Text("  Last Hit Time: %.2f sec", info.lastHitTime);
                ImGui::Separator();

                ImGui::PopID();
            }
        }
    );
}
