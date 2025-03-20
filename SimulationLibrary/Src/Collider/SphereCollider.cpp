#include "pch.h"
#include "Collider/SphereCollider.h"

#include <iostream>


bool SphereCollider::CheckCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        // Sphere-Sphere Collision
        DirectX::XMVECTOR centerA = XMLoadFloat3(&mAttachedObject->Position);
        DirectX::XMVECTOR centerB = XMLoadFloat3(&sphereCollider->mAttachedObject->Position);
        DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(centerA, centerB);

        float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(diff));
        float radiusSum = Radius + sphereCollider->Radius;

        return distanceSq <= (radiusSum * radiusSum);
    }
    else if (auto* planeCollider = dynamic_cast<PlaneCollider*>(other))
    {
        // Load sphere position
        DirectX::XMVECTOR sphereCenter = XMLoadFloat3(&mAttachedObject->Position);

        // Load plane transform (position, normal, rotation)
        DirectX::XMVECTOR planePosition = XMLoadFloat3(&planeCollider->Position);
        DirectX::XMVECTOR planeNormal = XMLoadFloat3(&planeCollider->Normal);
        DirectX::XMVECTOR planeRotation = XMLoadFloat4(&planeCollider->Rotation);

        // Apply rotation to plane normal
        planeNormal = DirectX::XMVector3Rotate(planeNormal, planeRotation);

        // Compute signed distance from sphere to plane
        DirectX::XMVECTOR sphereToPlane = DirectX::XMVectorSubtract(sphereCenter, planePosition);
        float signedDistance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToPlane, planeNormal));

        // Check if the sphere is colliding with the plane
        return signedDistance <= Radius && signedDistance >= -Radius;
    }
    return false;
}

void SphereCollider::ResolveCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        // Sphere-Sphere Collision Response
        DirectX::XMVECTOR centerA = XMLoadFloat3(&mAttachedObject->Position);
        DirectX::XMVECTOR centerB = XMLoadFloat3(&sphereCollider->mAttachedObject->Position);
        DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(centerA, centerB);
        DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(diff);

        float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diff));
        float radiusSum = Radius + sphereCollider->Radius;
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f) // Ensure there's penetration
        {
            DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, penetrationDepth * 0.5f);

            DirectX::XMVECTOR newPosA = DirectX::XMVectorAdd(centerA, correction);
            DirectX::XMVECTOR newPosB = DirectX::XMVectorSubtract(centerB, correction);

            XMStoreFloat3(&mAttachedObject->Position, newPosA);
            XMStoreFloat3(&sphereCollider->mAttachedObject->Position, newPosB);
        }

        // Reflect velocity
        DirectX::XMVECTOR velocityA = XMLoadFloat3(&mAttachedObject->Velocity);
        DirectX::XMVECTOR velocityB = XMLoadFloat3(&sphereCollider->mAttachedObject->Velocity);

        DirectX::XMVECTOR vA_normal = DirectX::XMVectorScale(normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocityA, normal)));
        DirectX::XMVECTOR vB_normal = DirectX::XMVectorScale(normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocityB, normal)));

        DirectX::XMVECTOR newVelocityA = DirectX::XMVectorSubtract(velocityA, DirectX::XMVectorScale(vA_normal, 2.0f));
        DirectX::XMVECTOR newVelocityB = DirectX::XMVectorSubtract(velocityB, DirectX::XMVectorScale(vB_normal, 2.0f));

        XMStoreFloat3(&mAttachedObject->Velocity, newVelocityA);
        XMStoreFloat3(&sphereCollider->mAttachedObject->Velocity, newVelocityB);
    }

    else if (auto* planeCollider = dynamic_cast<PlaneCollider*>(other))
    {
        // Sphere-Plane Collision Response
        DirectX::XMVECTOR velocity = XMLoadFloat3(&mAttachedObject->Velocity);
        DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&planeCollider->Normal);

        // Reflect velocity
        DirectX::XMVECTOR reflectedVelocity = DirectX::XMVectorSubtract(
            velocity,
            DirectX::XMVectorScale(normal, 2 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocity, normal)))
        );

        XMStoreFloat3(&mAttachedObject->Velocity, reflectedVelocity);

        // Push sphere out of plane to prevent sinking
        DirectX::XMVECTOR sphereCenter = XMLoadFloat3(&mAttachedObject->Position);
        float distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(normal, sphereCenter)) + planeCollider->D;

        if (std::abs(distance) < Radius)
        {
            DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, Radius - distance);
            DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(sphereCenter, correction);
            XMStoreFloat3(&mAttachedObject->Position, newPosition);
        }
    }
}
