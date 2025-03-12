#pragma once

#include "Physics/Particle.h"


//~ Base Force Generator
class IParticleEffectInterface
{
public:
	virtual void UpdateForce(XParticle* particle, float duration) = 0;
};
