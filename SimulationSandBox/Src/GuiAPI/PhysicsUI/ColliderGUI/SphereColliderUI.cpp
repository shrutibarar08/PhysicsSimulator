#include "GuiAPI/PhysicsUI/ColliderGUI/SphereColliderUI.h"
#include "imgui/imgui.h"


void SphereColliderUI::OnUpdateGUI()
{
    ImGui::SeparatorText("Sphere Collider");
    ImGui::Text("Properties:");
    ImGui::DragFloat("Elastic", &mSphereCollider.Elastic, 0.01f,
        0.0f, 1.0f, "%0.002f");

    ImGui::InputFloat("Radius", &mSphereCollider.Radius);
    ImGui::Checkbox("Static", &mSphereCollider.bStatic);
}

void SphereColliderUI::LoadFromJson(const nlohmann::json& jsonData)
{
}

nlohmann::json SphereColliderUI::SaveToJson()
{
	return nlohmann::json();
}

ICollider* SphereColliderUI::Collider()
{
	return &mSphereCollider;
}
