#pragma once

#include "ParticleSystem.h"
#include "GuiAPI/PhysicsUI/IPhysicsUI.h"

struct ParticleEffectData
{
	int id;
	std::string name;
	std::unique_ptr<ParticleEffectInterface> effect;
};

class ParticleSystemUI final: public IPhysicsUI
{
public:
	ParticleSystemUI() = default;

	void OnUpdateGUI() override;
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() override;
	~ParticleSystemUI() override = default;

	ParticleSystem* GetParticleSystem();

private:
	void CreationOptionUI();

private:
	ParticleSystem mParticleSystem{};
	std::vector<ParticleEffectData> mParticleEffects;
	int mEffectIndex{ -1 };
};

