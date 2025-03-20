#pragma once

#include "Compute/Integration.h"
#include "Compute/Particle.h"
#include "Compute/ParticleEffect.h"

#include <memory>
#include <unordered_map>

class ParticleSystem
{
public:
	ParticleSystem();

	void SetIntegration(Phyx::EIntegration type);
	void Update(Particle& particle, float deltaTime);

	int AddEffect(ParticleEffectInterface* effect);
	void RemoveEffect(int effectId);

	~ParticleSystem() = default;
private:
	std::unique_ptr<IntegrationInterface> mIntegration{ nullptr };
	std::unordered_map<int, ParticleEffectInterface*> mParticleEffects;
	int mCounter{ 0 };
};
