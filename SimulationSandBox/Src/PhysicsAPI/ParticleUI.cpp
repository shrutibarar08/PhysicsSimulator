#include "GuiAPI/PhysicsUI/ParticleUI.h"
#include "imgui/imgui.h"


void ParticleUI::OnUpdateGUI()
{
    ImGui::Text("Properties");
    // Mass
    ImGui::DragFloat("Mass", &mParticle.mMass, 0.1f, 0.001f, 10000.0f);
    mParticle.SetMass(mParticle.mMass);  // Ensure setter is called

    // Velocity
    ImGui::DragFloat3("Velocity", reinterpret_cast<float*>(&mParticle.Velocity), 0.1f);
    mParticle.SetVelocity(mParticle.Velocity);

    // Acceleration
    ImGui::DragFloat3("Acceleration", reinterpret_cast<float*>(&mParticle.Acceleration), 0.1f);
    mParticle.SetAcceleration(mParticle.Acceleration);

    // Accumulated Force
    ImGui::DragFloat3("Force", reinterpret_cast<float*>(&mParticle.AccumulatedForce), 0.1f);

    // Damping Energy
    ImGui::DragFloat("Damping", &mParticle.mDamping,
        0.01f, 0.f,
        1.f, "%0.02f");
}

void ParticleUI::LoadFromJson(const nlohmann::json& jsonData)
{
    if (jsonData.contains("Mass") && jsonData["Mass"].is_number_float())
        mParticle.SetMass(jsonData["Mass"]);

    if (jsonData.contains("Velocity") && jsonData["Velocity"].is_array() && jsonData["Velocity"].size() == 3)
        mParticle.SetVelocity({ jsonData["Velocity"][0], jsonData["Velocity"][1], jsonData["Velocity"][2] });

    if (jsonData.contains("Acceleration") && jsonData["Acceleration"].is_array() && jsonData["Acceleration"].size() == 3)
        mParticle.SetAcceleration({ jsonData["Acceleration"][0], jsonData["Acceleration"][1], jsonData["Acceleration"][2] });

    if (jsonData.contains("Force") && jsonData["Force"].is_array() && jsonData["Force"].size() == 3)
        mParticle.AccumulatedForce = { jsonData["Force"][0], jsonData["Force"][1], jsonData["Force"][2] };

    if (jsonData.contains("Damping") && jsonData["Damping"].is_number_float())
        mParticle.mDamping = jsonData["Damping"];
}

nlohmann::json ParticleUI::SaveToJson()
{
    return {
        {"Mass", mParticle.mMass},
        {"Velocity", {mParticle.Velocity.x, mParticle.Velocity.y, mParticle.Velocity.z}},
        {"Acceleration", {mParticle.Acceleration.x, mParticle.Acceleration.y, mParticle.Acceleration.z}},
        {"Force", {mParticle.AccumulatedForce.x, mParticle.AccumulatedForce.y, mParticle.AccumulatedForce.z}},
        {"Damping", mParticle.mDamping}
    };
}

Particle* ParticleUI::GetParticle()
{
	return &mParticle;
}
