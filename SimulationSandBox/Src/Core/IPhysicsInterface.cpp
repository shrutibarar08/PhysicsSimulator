#include "Core/Components/PhysicsObject.h"


void PhysicsObject::ApplyEffect(IParticleEffectInterface* effect)
{
	mActiveParticleEffect.emplace_back(effect);
}

void PhysicsObject::RemoveEffect(IParticleEffectInterface* effect)
{
	for (auto iter = mActiveParticleEffect.begin(); iter != mActiveParticleEffect.end(); iter++)
	{
		if (*iter == effect)
		{
			mActiveParticleEffect.erase(iter);
			return;
		}
	}
}

void PhysicsObject::InitParticleUpdateGUI()
{
	// TOBE Implemented
}
