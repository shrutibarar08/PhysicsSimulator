#pragma once

#include "Compute/Particle.h"
#include "GuiAPI/PhysicsUI/IPhysicsUI.h"


class ParticleUI final: public IPhysicsUI
{
public:
	ParticleUI() = default;

	void OnUpdateGUI() override;
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() override;
	~ParticleUI() override = default;

	Particle* GetParticle();

private:
	Particle mParticle{};
	DirectX::XMFLOAT3 mForceTobeApplied{ 0.0f, 0.0f, 0.0f };
};
