#pragma once

#include "Physics/ParticleEffects/XParticleEffectInterface.h"
#include <vector>


//~ Force Generators Container
namespace Phyx
{
	class XParticleEffectRegistration
	{
	public:
		void Add(XParticle* particle, IParticleEffectInterface* forceGen);
		void Remove(XParticle* particle, IParticleEffectInterface* forcegen);
		void Clear();
		void UpdateForces(float duration);

	protected:
		struct PARTICLE_EFFECT
		{
			XParticle* Particle;
			IParticleEffectInterface* ParticleForceGen;
		};

		//~ Container of initialized forces
		typedef std::vector<PARTICLE_EFFECT> XForceRegistry;
		XForceRegistry Registrations;
	};
}
