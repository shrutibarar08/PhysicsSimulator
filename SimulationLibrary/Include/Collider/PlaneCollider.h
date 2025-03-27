#pragma once

#include "Collider/ICollider.h"
#include "Compute/Particle.h"


class PlaneCollider : public ICollider
{
public:
    DirectX::XMFLOAT3 Normal{ 0.0f, 1.0f, 0.0f };
    float D{ 0.0f };

    bool CheckCollision(ICollider* other) override;
    void ResolveCollision(ICollider* other) override;
};
