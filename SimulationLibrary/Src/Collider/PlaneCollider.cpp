#include "pch.h"
#include "Collider/PlaneCollider.h"
#include "Collider/SphereCollider.h"

#include <DirectXMath.h>

using namespace DirectX;

bool PlaneCollider::CheckCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        // Sphere-Plane Collision Detection
        XMVECTOR sphereCenter = XMLoadFloat3(&sphereCollider->mAttachedObject->Position);
        XMVECTOR planeNormal = XMLoadFloat3(&Normal);

        // Calculate signed distance from the sphere center to the plane
        float distance = XMVectorGetX(XMVector3Dot(planeNormal, sphereCenter)) + D;

        return std::abs(distance) <= sphereCollider->Radius;
    }

    return false; 
}

void PlaneCollider::ResolveCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        // Sphere-Plane Collision Response
        XMVECTOR velocity = XMLoadFloat3(&sphereCollider->mAttachedObject->Velocity);
        XMVECTOR planeNormal = XMLoadFloat3(&Normal);

        // Reflect velocity
        XMVECTOR reflectedVelocity = XMVectorSubtract(
            velocity, XMVectorScale(planeNormal, 2 * XMVectorGetX(XMVector3Dot(velocity, planeNormal)))
        );

        XMStoreFloat3(&sphereCollider->mAttachedObject->Velocity, reflectedVelocity);
    }
}
