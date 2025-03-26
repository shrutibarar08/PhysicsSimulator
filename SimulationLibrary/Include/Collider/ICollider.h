#pragma once
#include <string>

#include "Compute/Particle.h"


class ICollider
{
public:
    ICollider() = default;

    void AttachParticle(Particle* attachParticle);
    virtual bool CheckCollision(ICollider* other) = 0;
    virtual void ResolveCollision(ICollider* other) = 0;
    std::string GetColliderName();
    void SetColliderName(const std::string& name);
    virtual ~ICollider() = default;

    bool AreStatic(const ICollider* other) const;

    Particle* mAttachedObject; // The mAttachedObject this collider is attached to

	//~ Members
    float Elastic{ 1.0f };
    bool bStatic{ false };

private:
    std::string mName{ "UNKNOWN" };
};
