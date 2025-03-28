#include "GuiAPI/PhysicsUI/ColliderGUI/CubeColliderUI.h"
#include "imgui/imgui.h"


void CubeColliderUI::Update(float deltaTime)
{
    IColliderUI::Update(deltaTime);
    mCubeCollider.UpdateTransform();
}

void CubeColliderUI::ChildUpdateGUI()
{
    ImGui::SeparatorText("Cube Collider");
    ImGui::Text("Properties:");
    ImGui::DragFloat("Elastic", &mCubeCollider.Elastic, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat3("Scale", &mCubeCollider.mScale.x, 0.01f, 0.0f, 100.0f, "%0.002f");
    ImGui::Checkbox("Static", &mCubeCollider.bStatic);
}

void CubeColliderUI::LoadFromJson(const nlohmann::json& jsonData)
{
    if (jsonData.contains("Scale") && jsonData["Scale"].is_array() && jsonData["Scale"].size() == 3)
    {
        mCubeCollider.mScale.x = jsonData["Scale"][0];
        mCubeCollider.mScale.y = jsonData["Scale"][1];
        mCubeCollider.mScale.z = jsonData["Scale"][2];
    }

    if (jsonData.contains("Static") && jsonData["Static"].is_boolean())
        mCubeCollider.bStatic = jsonData["Static"];
}

nlohmann::json CubeColliderUI::SaveToJson()
{
    return {
        {"Name", mCubeCollider.GetColliderName()},
        {"Scale", {mCubeCollider.mScale.x, mCubeCollider.mScale.y, mCubeCollider.mScale.z}},
        {"Static", mCubeCollider.bStatic}
    };
}

ICollider* CubeColliderUI::Collider()
{
	return &mCubeCollider;
}
