#pragma once
#include "Compute/Particle.h"


class ICollider
{
public:
    ICollider() = default;

    void AttachParticle(Particle* attachParticle);
    virtual bool CheckCollision(ICollider* other) = 0;
    virtual void ResolveCollision(ICollider* other) = 0;
    virtual ~ICollider() = default;

    Particle* mAttachedObject; // The mAttachedObject this collider is attached to
};
