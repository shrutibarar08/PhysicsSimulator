#include "pch.h"
#include "Collider/SphereCollider.h"
#include "Collider/CubeCollider.h"

#include <iostream>


bool SphereCollider::CheckCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        return CheckSphereCollision(sphereCollider);
    }
    if (auto* cubeCollider = dynamic_cast<CubeCollider*>(other))
    {
        return CheckCubeCollision(cubeCollider);
    }
    return false;
}

void SphereCollider::ResolveCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        HandleSphereCollision(sphereCollider);
        return;
    }

    if (auto* cubeCollider = dynamic_cast<CubeCollider*>(other))
    {
        HandleCubeCollision(cubeCollider);
        return;
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
        RegisterCollision(other, normal);
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

    // Compute impulse scalar
    float impulseScalar = -(1 + Elastic) * velAlongNormal;
    impulseScalar /= (1.0f / massA + 1.0f / massB);

    // Compute impulse vector
    DirectX::XMVECTOR impulse = DirectX::XMVectorScale(normal, impulseScalar);

    // Apply impulse only if the particle is not resting
    if (!bStatic)
    {
        DirectX::XMVECTOR newVelocityA = DirectX::XMVectorAdd(velocityA, DirectX::XMVectorScale(impulse, 1.0f / massA));
        newVelocityA = DirectX::XMVectorScale(newVelocityA, mAttachedObject->mDamping); // Apply damping
    	XMStoreFloat3(&mAttachedObject->Velocity, newVelocityA);

        if (mAttachedObject->IsResting())  // Check if the object is resting
        {
            mAttachedObject->Velocity.y = 0.0f;
        }
    }

    if (!other->bStatic)
    {
        if (other->mAttachedObject->IsResting()) 
        {
            other->mAttachedObject->Velocity = { 0.0f, 0.0f, 0.0f };  // Snap to zero
        }
        else
        {
            DirectX::XMVECTOR newVelocityB = DirectX::XMVectorSubtract(velocityB, DirectX::XMVectorScale(impulse, 1.0f / massB));
            newVelocityB = DirectX::XMVectorScale(newVelocityB, other->mAttachedObject->mDamping); // Apply damping
            XMStoreFloat3(&other->mAttachedObject->Velocity, newVelocityB);
        }
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


#pragma region CUBE_COLLISION

bool SphereCollider::CheckCubeCollision(class CubeCollider* other) const
{
    // Load sphere center and radius
    DirectX::XMVECTOR sphereCenter = XMLoadFloat3(&mAttachedObject->Position);
    float sphereRadius = Radius;

    // Load cube properties
    DirectX::XMVECTOR cubeCenter = XMLoadFloat3(&other->mAttachedObject->Position);
    DirectX::XMVECTOR cubeScale = XMLoadFloat3(&other->mScale); // Half-extents

    // Calculate closest point on the cube to the sphere
    DirectX::XMVECTOR minBounds = DirectX::XMVectorSubtract(cubeCenter, cubeScale);
    DirectX::XMVECTOR maxBounds = DirectX::XMVectorAdd(cubeCenter, cubeScale);

    DirectX::XMVECTOR closestPoint = DirectX::XMVectorClamp(sphereCenter, minBounds, maxBounds);

    // Compute squared distance from sphere center to closest point
    DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(sphereCenter, closestPoint);
    float squaredDistance = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(diff));

    // Collision occurs if the squared distance is less than the squared radius
    return squaredDistance <= (sphereRadius * sphereRadius);
}

void SphereCollider::HandleCubeCollision(CubeCollider* cube)
{
    DirectX::XMVECTOR normal;
    float penetrationDepth;

    if (ComputeSphereCubeCollisionInfo(cube, normal, penetrationDepth))
    {
        RegisterCollision(cube, normal);
        ResolveSpherePenetration(normal, penetrationDepth);
        ResolveSphereVelocity(normal);
    }
}

void SphereCollider::ResolveSpherePenetration(
    const DirectX::XMVECTOR& normal, float penetrationDepth) const
{
    if (bStatic) return;

    // Move sphere out of the cube along the normal
    DirectX::XMVECTOR correction = DirectX::XMVectorScale(normal, penetrationDepth);
    DirectX::XMVECTOR newPos = DirectX::XMLoadFloat3(&mAttachedObject->Position);
    newPos = DirectX::XMVectorAdd(newPos, correction);

    XMStoreFloat3(&mAttachedObject->Position, newPos);
}

void SphereCollider::ResolveSphereVelocity(const DirectX::XMVECTOR& normal) const
{
    DirectX::XMVECTOR velocity = XMLoadFloat3(&mAttachedObject->Velocity);

    // Compute velocity along the normal
    float velAlongNormal = DirectX::XMVectorGetX(DirectX::XMVector3Dot(velocity, normal));

    if (velAlongNormal > 0) return; // Moving away, no resolution needed

    // Reflect velocity for bounce effect
    DirectX::XMVECTOR newVelocity = DirectX::XMVectorSubtract(
        velocity, DirectX::XMVectorScale(normal, 2.0f * velAlongNormal)
    );
	XMStoreFloat3(&mAttachedObject->Velocity, newVelocity);

    if (mAttachedObject->IsResting())  // Check if the object is resting
    {
        mAttachedObject->Velocity.y = 0.0f;
    }
}

bool SphereCollider::ComputeSphereCubeCollisionInfo(
    const CubeCollider* cube,
    DirectX::XMVECTOR& normal,
    float& penetrationDepth) const
{
    DirectX::XMVECTOR spherePos = DirectX::XMLoadFloat3(&mAttachedObject->Position);
    DirectX::XMVECTOR cubePos = DirectX::XMLoadFloat3(&cube->mAttachedObject->Position);
    DirectX::XMVECTOR cubeHalfExtents = DirectX::XMLoadFloat3(&cube->mScale);

    // Find the closest point on the cube to the sphere
    DirectX::XMVECTOR closestPoint = DirectX::XMVectorClamp(
        spherePos,
        DirectX::XMVectorSubtract(cubePos, cubeHalfExtents), // Min corner
        DirectX::XMVectorAdd(cubePos, cubeHalfExtents)       // Max corner
    );

    // Compute the vector from closest point to sphere center
    normal = DirectX::XMVectorSubtract(spherePos, closestPoint);
    float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(normal));

    if (distanceSq == 0.0f)
    {
        // Sphere center is inside the cube
        normal = DirectX::XMVectorSet(0, 1, 0, 0); // Default upward normal
        penetrationDepth = Radius;
        return true;
    }

    // Normalize normal
    normal = DirectX::XMVector3Normalize(normal);

    // Compute penetration depth
    float distance = sqrtf(distanceSq);
    penetrationDepth = Radius - distance;

    return penetrationDepth > 0.0f;
}


#pragma endregion
