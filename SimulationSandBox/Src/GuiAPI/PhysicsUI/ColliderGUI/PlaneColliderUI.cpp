#include "GuiAPI/PhysicsUI/ColliderGUI/PlaneColliderUI.h"
#include "imgui/imgui.h"


ICollider* PlaneColliderUI::Collider()
{
	return &mPlaneCollider;
}

void PlaneColliderUI::OnUpdateGUI()
{
    ImGui::SeparatorText("Plane Collider");
    ImGui::Text("Properties:");
    ImGui::DragFloat("Elastic", &mPlaneCollider.Elastic,
        0.01f, 0.0f, 1.0f, "%0.002f");

    ImGui::InputFloat("Off Set", &mPlaneCollider.D);
    ImGui::DragFloat3("Normal", &mPlaneCollider.Normal.x);
    ImGui::Checkbox("Static", &mPlaneCollider.bStatic);
}

void PlaneColliderUI::LoadFromJson(const nlohmann::json& jsonData)
{
}

nlohmann::json PlaneColliderUI::SaveToJson()
{
	return nlohmann::json();
}
