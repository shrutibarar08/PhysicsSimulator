#include "pch.h"
#include "Physics/ParticleForceGen.h"

void Phyx::XParticleEffectRegistration::Add(XParticle* particle, IParticleEffectInterface* forceGen)
{
	Registrations.emplace_back(PARTICLE_EFFECT{ particle, forceGen });
}

void Phyx::XParticleEffectRegistration::Remove(XParticle* particle, IParticleEffectInterface* forcegen)
{
	for (auto iter = Registrations.begin(); iter != Registrations.end(); ++iter)
	{
		if (iter->Particle == particle && iter->ParticleForceGen == forcegen)
		{
			Registrations.erase(iter);
			return;
		}
	}
}

void Phyx::XParticleEffectRegistration::Clear()
{
	Registrations.clear();
}

void Phyx::XParticleEffectRegistration::UpdateForces(float duration)
{
	for (auto iter = Registrations.begin(); iter != Registrations.end(); iter++)
	{
		iter->ParticleForceGen->UpdateForce(iter->Particle, duration);
	}
}
