#include "pch.h"
#include "Collider/ICollider.h"

void ICollider::AttachParticle(Particle* attachParticle)
{
	mAttachedObject = attachParticle;
}

std::string ICollider::GetColliderName()
{
	return mName;
}

void ICollider::SetColliderName(const std::string& name)
{
	mName = name;
}

bool ICollider::AreStatic(const ICollider* other) const
{
	return bStatic && other->bStatic;
}
