#pragma once
#include "ParticleSystem.h"
#include "GuiAPI/PhysicsUI/ParticleUI.h"
#include "PhysicsAPI/DefineCollider.h"

struct ParticleEffectData
{
	int id;
	std::string name;
	std::unique_ptr<ParticleEffectInterface> effect;
};

class PhysicsObject
{
public:
	PhysicsObject() = default;

	void InitGUI();
	void Update(float deltaTime);
	void ToggleEffects();

	nlohmann::json SaveToJson();
	void LoadFromJson(const nlohmann::json& json);

private:
	void InitParticleUpdateGUI();
	void InitColliderPopUp();
	void InitColliderUpdateGUI();

	void InitParticleEffectPopUp();
	void InitParticleEffectsUpdateGUI();

public:
	std::unique_ptr<IColliderUI> mCollider{ nullptr };
	ParticleUI mParticle{};
	ParticleSystem mParticleSystem{};

private:
	std::vector<ParticleEffectData> mParticleEffects;
	int mColliderIndex{ -1 };
	bool mbEffects{ true };
};
