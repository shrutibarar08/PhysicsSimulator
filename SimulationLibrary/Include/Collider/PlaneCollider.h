#pragma once

#include "Collider/ICollider.h"
#include "Compute/Particle.h"


class PlaneCollider : public ICollider
{
public:
    DirectX::XMFLOAT3 Normal{ 0.0f, 1.0f, 0.0f };
    float D{ 0.0f };
    DirectX::XMFLOAT3 Position{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 Scale{ 10.0f, 1.0f, 10.0f };
    DirectX::XMFLOAT4 Rotation{ 0.0f, 0.0f, 0.0f, 1.0f };

    void SetTransform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT4& rotation)
    {
        Position = position;
        Scale = scale;
        Rotation = rotation;
    }

    bool CheckCollision(ICollider* other) override;
    void ResolveCollision(ICollider* other) override;
};
