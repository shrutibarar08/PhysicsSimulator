#include "pch.h"
#include "Collider/SphereCollider.h"

#include <iostream>


bool SphereCollider::CheckCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        return CheckSphereCollision(sphereCollider);
    }
    else if (auto* planeCollider = dynamic_cast<PlaneCollider*>(other))
    {
        return CheckPlaneCollision(planeCollider);
    }
    return false;
}

void SphereCollider::ResolveCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        HandleSphereCollision(sphereCollider);
    }
    else if (auto* planeCollider = dynamic_cast<PlaneCollider*>(other))
    {
        HandlePlaneCollision(planeCollider);
    }
}

// ------------------- COLLISION DETECTION ------------------------------

#pragma region SPHERE_COLLISION

void SphereCollider::HandleSphereCollision(SphereCollider* other)
{
    DirectX::XMVECTOR centerA = DirectX::XMLoadFloat3(&mAttachedObject->Position);
    DirectX::XMVECTOR centerB = DirectX::XMLoadFloat3(&other->mAttachedObject->Position);

    DirectX::XMVECTOR normal;
    float penetrationDepth;

    if (ComputeSphereCollisionInfo(centerA, centerB, normal, penetrationDepth, other))
    {
        ResolveSpherePenetration(centerA, centerB, normal, penetrationDepth, other);
        ResolveSphereVelocity(normal, other);
    }
}

bool SphereCollider::CheckSphereCollision(SphereCollider* other) const
{
    DirectX::XMVECTOR centerA = XMLoadFloat3(&mAttachedObject->Position);
    DirectX::XMVECTOR centerB = XMLoadFloat3(&other->mAttachedObject->Position);

    DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(centerA, centerB);
    float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(diff));
    float radiusSum = Radius + other->Radius;

    return distanceSq <= (radiusSum * radiusSum);
}

bool SphereCollider::ComputeSphereCollisionInfo(
    const DirectX::XMVECTOR& centerA, const DirectX::XMVECTOR& centerB,
    DirectX::XMVECTOR& normal, float& penetrationDepth, SphereCollider* other) const
{
    DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(centerA, centerB);
    float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(diff));
    float radiusSum = Radius + other->Radius;

    if (distanceSq > (radiusSum * radiusSum))
        return false;  // No collision

    float distance = std::sqrt(distanceSq);
    normal = DirectX::XMVectorDivide(diff, DirectX::XMVectorReplicate(distance));
    penetrationDepth = radiusSum - distance;
    return true;
}

void SphereCollider::ResolveSpherePenetration(
    const DirectX::XMVECTOR& centerA, const DirectX::XMVECTOR& centerB,
    const DirectX::XMVECTOR& normal, float penetrationDepth,
    SphereCollider* other) const
{
    if (AreStatic(other)) return;

    if (bStatic)
    {
        // Move only the dynamic object
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, penetrationDepth);
        XMStoreFloat3(&other->mAttachedObject->Position, DirectX::XMVectorSubtract(centerB, correction));
        return;
    }
	if (other->bStatic)
    {
        // Move only this object
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, penetrationDepth);
        XMStoreFloat3(&mAttachedObject->Position, DirectX::XMVectorAdd(centerA, correction));
        return;
    }

    float totalMass = mAttachedObject->mMass + other->mAttachedObject->mMass;

    // Compute displacement factors based on mass
    float moveA = other->mAttachedObject->mMass / totalMass;
    float moveB = mAttachedObject->mMass / totalMass;

    // Distribute penetration correction
    DirectX::XMVECTOR correctionA = DirectX::XMVectorScale(normal, penetrationDepth * moveA);
    DirectX::XMVECTOR correctionB = DirectX::XMVectorScale(normal, penetrationDepth * moveB);

    // Apply corrections
    XMStoreFloat3(&mAttachedObject->Position, DirectX::XMVectorAdd(centerA, correctionA));
    XMStoreFloat3(&other->mAttachedObject->Position, DirectX::XMVectorSubtract(centerB, correctionB));
}

void SphereCollider::ResolveSphereVelocity(const DirectX::XMVECTOR& normal, const SphereCollider* other) const
{
    if (AreStatic(other)) return;

    DirectX::XMVECTOR velocityA = XMLoadFloat3(&mAttachedObject->Velocity);
    DirectX::XMVECTOR velocityB = XMLoadFloat3(&other->mAttachedObject->Velocity);

    float massA = mAttachedObject->mMass;
    float massB = other->mAttachedObject->mMass;

    // Compute relative velocity
    DirectX::XMVECTOR relativeVelocity = DirectX::XMVectorSubtract(velocityA, velocityB);

    // Compute velocity along the normal
    float velAlongNormal = DirectX::XMVectorGetX(DirectX::XMVector3Dot(relativeVelocity, normal));

    if (velAlongNormal > 0) return; // Objects are separating

    // Compute impulse scalar correctly
    float impulseScalar = -(1 + Elastic) * velAlongNormal;
    impulseScalar /= (1.0f / massA + 1.0f / massB);

    // Compute impulse vector
    DirectX::XMVECTOR impulse = DirectX::XMVectorScale(normal, impulseScalar);

    // Apply impulse to non-static objects
    if (!bStatic)
    {
        DirectX::XMVECTOR newVelocityA = DirectX::XMVectorAdd(velocityA, DirectX::XMVectorScale(impulse, 1.0f / massA));
        newVelocityA = DirectX::XMVectorScale(newVelocityA, mAttachedObject->mDamping); // Apply damping
        XMStoreFloat3(&mAttachedObject->Velocity, newVelocityA);
    }
    if (!other->bStatic)
    {
        DirectX::XMVECTOR newVelocityB = DirectX::XMVectorSubtract(velocityB, DirectX::XMVectorScale(impulse, 1.0f / massB));
        newVelocityB = DirectX::XMVectorScale(newVelocityB, other->mAttachedObject->mDamping); // Apply damping
        XMStoreFloat3(&other->mAttachedObject->Velocity, newVelocityB);
    }
}

//~ For immovable objects
void SphereCollider::ComputeSphereVelocityChange(
    const DirectX::XMVECTOR& velocityA, const DirectX::XMVECTOR& velocityB,
    const DirectX::XMVECTOR& normal, DirectX::XMVECTOR& newVelocityA, DirectX::XMVECTOR& newVelocityB) const
{
    DirectX::XMVECTOR vA_proj = DirectX::XMVectorScale(normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocityA, normal)));
    DirectX::XMVECTOR vB_proj = DirectX::XMVectorScale(normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocityB, normal)));

    newVelocityA = DirectX::XMVectorSubtract(velocityA, DirectX::XMVectorScale(vA_proj, 2.0f));
    newVelocityB = DirectX::XMVectorSubtract(velocityB, DirectX::XMVectorScale(vB_proj, 2.0f));
}

#pragma endregion

#pragma region PLANE_COLLISION

void SphereCollider::HandlePlaneCollision(PlaneCollider* other)
{
    DirectX::XMVECTOR velocity = XMLoadFloat3(&mAttachedObject->Velocity);
    DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&other->Normal);

    ReflectVelocity(normal);
    CorrectSpherePosition(normal, other);
}

bool SphereCollider::CheckPlaneCollision(PlaneCollider* planeCollider)
{
    // Load sphere position
    DirectX::XMVECTOR sphereCenter = XMLoadFloat3(&mAttachedObject->Position);

    // Load plane transform (position, normal, rotation)
    DirectX::XMVECTOR planePosition = DirectX::XMLoadFloat3(&planeCollider->Position);
    DirectX::XMVECTOR planeNormal = DirectX::XMLoadFloat3(&planeCollider->Normal);
    DirectX::XMVECTOR planeRotation = DirectX::XMLoadFloat4(&planeCollider->Rotation);

    // Apply rotation to plane normal
    planeNormal = DirectX::XMVector3Rotate(planeNormal, planeRotation);

    // Compute signed distance from sphere to plane
    DirectX::XMVECTOR sphereToPlane = DirectX::XMVectorSubtract(sphereCenter, planePosition);
    float signedDistance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToPlane, planeNormal));

    // Check if the sphere is colliding with the plane
    return signedDistance <= Radius && signedDistance >= -Radius;
}

void SphereCollider::ReflectVelocity(const DirectX::XMVECTOR& normal)
{
    DirectX::XMVECTOR velocity = XMLoadFloat3(&mAttachedObject->Velocity);
    DirectX::XMVECTOR reflectedVelocity = DirectX::XMVectorSubtract(
        velocity,
        DirectX::XMVectorScale(normal, 2 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocity, normal)))
    );

    XMStoreFloat3(&mAttachedObject->Velocity, reflectedVelocity);
}

void SphereCollider::CorrectSpherePosition(const DirectX::XMVECTOR& normal,
    const PlaneCollider* planeCollider) const
{
    DirectX::XMVECTOR sphereCenter = XMLoadFloat3(&mAttachedObject->Position);
    float distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(normal, sphereCenter)) + planeCollider->D;

    if (std::abs(distance) < Radius)
    {
        DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, Radius - distance);
        DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(sphereCenter, correction);
        XMStoreFloat3(&mAttachedObject->Position, newPosition);
    }
}

#pragma endregion
