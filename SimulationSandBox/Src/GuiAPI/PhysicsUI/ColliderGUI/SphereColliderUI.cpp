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
    if (jsonData.contains("Elastic") && jsonData["Elastic"].is_number_float())
        mSphereCollider.Elastic = jsonData["Elastic"];

    if (jsonData.contains("Radius") && jsonData["Radius"].is_number_float())
        mSphereCollider.Radius = jsonData["Radius"];

    if (jsonData.contains("Static") && jsonData["Static"].is_boolean())
        mSphereCollider.bStatic = jsonData["Static"];
}

nlohmann::json SphereColliderUI::SaveToJson()
{
    return {
        {"Name", mSphereCollider.GetColliderName() },
        {"Elastic", mSphereCollider.Elastic},
        {"Radius", mSphereCollider.Radius},
        {"Static", mSphereCollider.bStatic}
    };
}

ICollider* SphereColliderUI::Collider()
{
	return &mSphereCollider;
}
