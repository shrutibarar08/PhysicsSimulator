#pragma once
#include "Physics/ParticleEffects/XParticleEffectInterface.h"

#include <vector>


class PhysicsObject
{
public:
	PhysicsObject() = default;

	void ApplyEffect(IParticleEffectInterface* effect);
	void RemoveEffect(IParticleEffectInterface* effect);

	void InitParticleUpdateGUI();

protected:
	XParticle mParticle;

private:
	std::vector<IParticleEffectInterface*> mActiveParticleEffect;

	//~ TODO Create Collision Effect
};
