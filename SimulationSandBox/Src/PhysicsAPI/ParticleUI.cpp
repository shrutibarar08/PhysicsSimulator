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
}

nlohmann::json ParticleUI::SaveToJson()
{
	return nlohmann::json();
}

Particle* ParticleUI::GetParticle()
{
	return &mParticle;
}
