#pragma once

#include "Collider/PlaneCollider.h"
#include "Collider/ICollider.h"
#include "Compute/Particle.h"


class SphereCollider : public ICollider
{
public:
    float Radius{ 1.f };

    SphereCollider() = default;

    void SetRadius(float radius) { Radius = radius; }

    bool CheckCollision(ICollider* other) override;
    void ResolveCollision(ICollider* other) override;
};
