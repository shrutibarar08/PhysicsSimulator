#pragma once
#include "ParticleSystem.h"
#include "Collider/ICollider.h"

#include <unordered_map>

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

	void InitColliderPopUp();
	void InitColliderUpdateGUI();

	void InitParticleEffectPopUp();
	void InitParticleUpdateGUI();

	void Update(float deltaTime);

	void ToggleEffects();

public:
	std::unique_ptr<ICollider> mCollider{ nullptr };
	Particle mParticle{};
	ParticleSystem mParticleSystem{};

private:
	std::vector<ParticleEffectData> mParticleEffects;
	int mColliderIndex{ -1 };
	bool mbEffects{ true };
};
