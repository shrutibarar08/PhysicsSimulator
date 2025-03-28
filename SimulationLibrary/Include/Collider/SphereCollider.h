#pragma once

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

private:
    //~ Handle Sphere Collision
    void HandleSphereCollision(SphereCollider* other);
    bool CheckSphereCollision(SphereCollider* other) const;

    void ResolveSpherePenetration(const DirectX::XMVECTOR& centerA,
        const DirectX::XMVECTOR& centerB,
        const DirectX::XMVECTOR& normal, 
        float penetrationDepth,
        SphereCollider* other) const;

    void ResolveSphereVelocity(const DirectX::XMVECTOR& normal,
        const SphereCollider* other) const;

    bool ComputeSphereCollisionInfo(
        const DirectX::XMVECTOR& centerA, const DirectX::XMVECTOR& centerB,
        DirectX::XMVECTOR& normal, float& penetrationDepth,
        SphereCollider* other) const;

    void ComputeSphereVelocityChange(
        const DirectX::XMVECTOR& velocityA, const DirectX::XMVECTOR& velocityB,
        const DirectX::XMVECTOR& normal, DirectX::XMVECTOR& newVelocityA,
        DirectX::XMVECTOR& newVelocityB) const;

    //~ Handle Cube Collision
    bool CheckCubeCollision(class CubeCollider* other) const;
    void HandleCubeCollision(CubeCollider* cube);
    void ResolveSpherePenetration(const DirectX::XMVECTOR& normal, float penetrationDepth) const;
    void ResolveSphereVelocity(const DirectX::XMVECTOR& normal) const;
    bool ComputeSphereCubeCollisionInfo(const CubeCollider* cube, DirectX::XMVECTOR& normal,
                                        float& penetrationDepth) const;
};
