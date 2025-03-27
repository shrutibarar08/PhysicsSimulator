#pragma once
#include "GuiAPI/PhysicsUI/ParticleSystemUI.h"
#include "GuiAPI/PhysicsUI/ParticleUI.h"
#include "PhysicsAPI/DefineCollider.h"



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

	void InitParticleEffectsUpdateGUI();

public:
	std::unique_ptr<IColliderUI> mCollider{ nullptr };
	ParticleUI mParticle{};
	ParticleSystemUI mParticleSystem{};

private:
	bool mbEffects{ true };
	int mColliderIndex{ -1 };
};
