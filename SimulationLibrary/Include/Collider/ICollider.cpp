#include "pch.h"
#include "ICollider.h"

void ICollider::AttachParticle(Particle* attachParticle)
{
	mAttachedObject = attachParticle;
}
