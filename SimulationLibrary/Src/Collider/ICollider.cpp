#include "pch.h"
#include "Collider/ICollider.h"

void ICollider::AttachParticle(Particle* attachParticle)
{
	mAttachedObject = attachParticle;
}
