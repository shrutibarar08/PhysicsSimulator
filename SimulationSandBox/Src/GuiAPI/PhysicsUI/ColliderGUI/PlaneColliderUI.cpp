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
    if (jsonData.contains("Elastic") && jsonData["Elastic"].is_number_float())
        mPlaneCollider.Elastic = jsonData["Elastic"];

    if (jsonData.contains("OffSet") && jsonData["OffSet"].is_number_float())
        mPlaneCollider.D = jsonData["OffSet"];

    if (jsonData.contains("Normal") && jsonData["Normal"].is_array() && jsonData["Normal"].size() == 3)
        mPlaneCollider.Normal = { jsonData["Normal"][0], jsonData["Normal"][1], jsonData["Normal"][2] };

    if (jsonData.contains("Static") && jsonData["Static"].is_boolean())
        mPlaneCollider.bStatic = jsonData["Static"];
}

nlohmann::json PlaneColliderUI::SaveToJson()
{
    return {
        {"Name", mPlaneCollider.GetColliderName() },
        {"Elastic", mPlaneCollider.Elastic},
        {"OffSet", mPlaneCollider.D},
        {"Normal", {mPlaneCollider.Normal.x, mPlaneCollider.Normal.y, mPlaneCollider.Normal.z}},
        {"Static", mPlaneCollider.bStatic}
    };
}
