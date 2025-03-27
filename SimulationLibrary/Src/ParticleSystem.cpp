#include "pch.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
	SetIntegration(Phyx::EIntegration::euler_integration);
}

void ParticleSystem::SetIntegration(Phyx::EIntegration type)
{
	switch (type)
	{
	case Phyx::EIntegration::euler_integration:
		mIntegration = std::make_unique<EulerIntegration>();
		break;
	case Phyx::EIntegration::semi_implicit_euler_integration:
		mIntegration = std::make_unique<SemiImplicitEulerIntegration>();
		break;
	default:
		break;
	}
}

void ParticleSystem::Update(Particle* particle, float deltaTime)
{
	for (auto& effect: mParticleEffects)
	{
		effect.second->ApplyEffect(particle, deltaTime);
	}

	mIntegration->Integrate(particle, deltaTime);
}

int ParticleSystem::AddEffect(ParticleEffectInterface* effect)
{
	mCounter++;
	mParticleEffects[mCounter] = effect;
	return mCounter;
}

void ParticleSystem::RemoveEffect(int effectId)
{
	mParticleEffects.erase(effectId);
}
