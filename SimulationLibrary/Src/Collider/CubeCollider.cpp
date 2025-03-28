#include "pch.h"
#include "Collider/CubeCollider.h"

#include <iostream>

#include "Collider/SphereCollider.h"


bool CubeCollider::CheckCollision(ICollider* other)
{
    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        // Cube-Sphere Collision Detection
        DirectX::XMVECTOR sphereCenter = DirectX::XMLoadFloat3(&sphereCollider->mAttachedObject->Position);
        DirectX::XMVECTOR cubeCenter = DirectX::XMLoadFloat3(&mAttachedObject->Position);
        DirectX::XMVECTOR cubeHalfSize = DirectX::XMVectorScale(XMLoadFloat3(&mScale), 0.5f);

        // Clamp sphere position to the closest point on the cube
        DirectX::XMVECTOR closestPoint = DirectX::XMVectorClamp(
            sphereCenter,
            DirectX::XMVectorSubtract(cubeCenter, cubeHalfSize), // Min bound
            DirectX::XMVectorAdd(cubeCenter, cubeHalfSize)       // Max bound
        );

        // Distance between sphere center and closest point
        DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(sphereCenter, closestPoint);
        float distSquared = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(diff));

        // Check if within sphere radius
        return distSquared <= (sphereCollider->Radius * sphereCollider->Radius);
    }

    if (auto* cubeCollider = dynamic_cast<CubeCollider*>(other))
    {
        // Cube-Cube (AABB) Collision Detection
        DirectX::XMVECTOR minA = DirectX::XMVectorSubtract(XMLoadFloat3(&mAttachedObject->Position), XMLoadFloat3(&mScale));
        DirectX::XMVECTOR maxA = DirectX::XMVectorAdd(XMLoadFloat3(&mAttachedObject->Position), XMLoadFloat3(&mScale));

        DirectX::XMVECTOR minB = DirectX::XMVectorSubtract(XMLoadFloat3(&cubeCollider->mAttachedObject->Position), XMLoadFloat3(&cubeCollider->mScale));
        DirectX::XMVECTOR maxB = DirectX::XMVectorAdd(XMLoadFloat3(&cubeCollider->mAttachedObject->Position), XMLoadFloat3(&cubeCollider->mScale));

        // AABB check (Separating Axis Theorem for AABBs)
        return !(DirectX::XMVectorGetX(minA) > DirectX::XMVectorGetX(maxB) || DirectX::XMVectorGetX(maxA) < DirectX::XMVectorGetX(minB) ||
	        DirectX::XMVectorGetY(minA) > DirectX::XMVectorGetY(maxB) || DirectX::XMVectorGetY(maxA) < DirectX::XMVectorGetY(minB) ||
	        DirectX::XMVectorGetZ(minA) > DirectX::XMVectorGetZ(maxB) || DirectX::XMVectorGetZ(maxA) < DirectX::XMVectorGetZ(minB));
    }

    return false;
}


void CubeCollider::ResolveCollision(ICollider* other)
{
    if (auto* cubeCollider = dynamic_cast<CubeCollider*>(other))
    {
        HandleCubeCollision(cubeCollider);
        return;
    }

    if (auto* sphereCollider = dynamic_cast<SphereCollider*>(other))
    {
        HandleSphereCollider(sphereCollider);
        return;
    }
}

void CubeCollider::UpdateTransform()
{
    using namespace DirectX;

    // Load scale, rotation, and position
    XMVECTOR scaleVec = XMLoadFloat3(&mScale);
    XMVECTOR rotationQuat = XMLoadFloat4(&mAttachedObject->Rotation);
    XMVECTOR positionVec = XMLoadFloat3(&mAttachedObject->Position);

    // Compute world transform
    XMMATRIX transformMatrix = XMMatrixScalingFromVector(scaleVec) *
        XMMatrixRotationQuaternion(rotationQuat) *
        XMMatrixTranslationFromVector(positionVec);

    // Store the matrix in mTransform
    XMStoreFloat4x4(&mTransform, transformMatrix);
}

void CubeCollider::HandleCubeCollision(CubeCollider* other)
{
    DirectX::XMVECTOR pushDir;
    float minOverlap;

    if (CheckOverlap(other, pushDir, minOverlap))
    {
        RegisterCollision(other, DirectX::XMVector3Normalize(pushDir));
        // **Always resolve penetration to prevent objects from going inside each other**
        ApplyPositionCorrection(other, pushDir, minOverlap);

        // **Each object should handle its own velocity update**
        if (!bStatic)
        {
            ApplyVelocityResponse(other, pushDir);
        }
    }
}

bool CubeCollider::CheckOverlap(const CubeCollider* other, DirectX::XMVECTOR& pushDirection, float& minOverlap) const
{
    // Convert positions and scales to XMVECTOR
    DirectX::XMVECTOR posA = DirectX::XMLoadFloat3(&mAttachedObject->Position);
    DirectX::XMVECTOR scaleA = DirectX::XMLoadFloat3(&mScale);

    DirectX::XMVECTOR posB = DirectX::XMLoadFloat3(&other->mAttachedObject->Position);
    DirectX::XMVECTOR scaleB = DirectX::XMLoadFloat3(&other->mScale);

    // Compute AABB min/max bounds
    DirectX::XMFLOAT3 minA, maxA, minB, maxB;
    DirectX::XMStoreFloat3(&minA, DirectX::XMVectorSubtract(posA, scaleA));
    DirectX::XMStoreFloat3(&maxA, DirectX::XMVectorAdd(posA, scaleA));

    DirectX::XMStoreFloat3(&minB, DirectX::XMVectorSubtract(posB, scaleB));
    DirectX::XMStoreFloat3(&maxB, DirectX::XMVectorAdd(posB, scaleB));

    // Compute overlaps on each axis
    float overlapX = std::min(maxA.x - minB.x, maxB.x - minA.x);
    float overlapY = std::min(maxA.y - minB.y, maxB.y - minA.y);
    float overlapZ = std::min(maxA.z - minB.z, maxB.z - minA.z);

    // Find the smallest overlap
    minOverlap = std::min({ overlapX, overlapY, overlapZ });

    // Determine push direction
    if (minOverlap == overlapX)
        pushDirection = DirectX::XMVectorSet((posA.m128_f32[0] < posB.m128_f32[0]) ? -1.0f : 1.0f, 0.0f, 0.0f, 0.0f);
    else if (minOverlap == overlapY)
        pushDirection = DirectX::XMVectorSet(0.0f, (posA.m128_f32[1] < posB.m128_f32[1]) ? -1.0f : 1.0f, 0.0f, 0.0f);
    else
        pushDirection = DirectX::XMVectorSet(0.0f, 0.0f, (posA.m128_f32[2] < posB.m128_f32[2]) ? -1.0f : 1.0f, 0.0f);

    return true;  // Since CheckCollision() already confirmed an overlap
}

void CubeCollider::ApplyPositionCorrection(CubeCollider* other, DirectX::XMVECTOR pushDirection, float minOverlap) const
{
    // Ensure pushDirection is normalized
    pushDirection = DirectX::XMVector3Normalize(pushDirection);

    // Compute the correction offset (half of the overlap)
    DirectX::XMVECTOR correction = DirectX::XMVectorScale(pushDirection, minOverlap * 0.5f);

    // **Only apply correction to this object (other will handle its own)**
    if (!bStatic)
    {
        mAttachedObject->Position = DirectX::XMFLOAT3(
            mAttachedObject->Position.x + DirectX::XMVectorGetX(correction),
            mAttachedObject->Position.y + DirectX::XMVectorGetY(correction),
            mAttachedObject->Position.z + DirectX::XMVectorGetZ(correction)
        );
    }
}

void CubeCollider::ApplyVelocityResponse(const CubeCollider* other, DirectX::XMVECTOR pushDirection) const
{
    // Normalize push direction
    pushDirection = DirectX::XMVector3Normalize(pushDirection);

    // Get current velocity as XMVECTOR
    DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(&mAttachedObject->Velocity);
    DirectX::XMVECTOR otherVelocity = DirectX::XMLoadFloat3(&other->mAttachedObject->Velocity);

    // Compute relative velocity
    DirectX::XMVECTOR relativeVelocity = DirectX::XMVectorSubtract(otherVelocity, velocity);

    // Compute velocity along the collision normal
    float velocityAlongNormal = DirectX::XMVectorGetX(DirectX::XMVector3Dot(relativeVelocity, pushDirection));

    // If moving away, no response needed
    if (velocityAlongNormal > 0.0f)
        return;

    // Coefficient of restitution (elasticity factor)
    float restitution = 0.5f; // 0 = fully inelastic, 1 = perfectly elastic

    // Compute impulse scalar
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;

    // Compute impulse vector
    DirectX::XMVECTOR impulse = DirectX::XMVectorScale(pushDirection, impulseScalar);

    // **Apply velocity only to THIS object, not the other**
    DirectX::XMStoreFloat3(&mAttachedObject->Velocity, DirectX::XMVectorAdd(velocity, impulse));
}

// ------------------ SPHERE COLLIDER --------------------------------
void CubeCollider::HandleSphereCollider(SphereCollider* other)
{
    DirectX::XMVECTOR pushDir;
    float minOverlap;

    if (CheckCubeSphereOverlap(other, pushDir, minOverlap))
    {
        RegisterCollision(other, DirectX::XMVector3Normalize(pushDir));

        if (!bStatic)
        {
            ApplyPositionCorrectionFromSphere(other, pushDir, minOverlap);
            ApplyVelocityResponseFromSphere(other, pushDir);
        }
    }
}

// **Check overlap between Cube and Sphere**
bool CubeCollider::CheckCubeSphereOverlap(const SphereCollider* other, DirectX::XMVECTOR& pushDirection, float& minOverlap) const
{
    // Cube AABB Bounds
    DirectX::XMVECTOR cubePos = DirectX::XMLoadFloat3(&mAttachedObject->Position);
    DirectX::XMVECTOR cubeScale = DirectX::XMLoadFloat3(&mScale);

    DirectX::XMFLOAT3 cubeMin, cubeMax;
    DirectX::XMStoreFloat3(&cubeMin, DirectX::XMVectorSubtract(cubePos, cubeScale));
    DirectX::XMStoreFloat3(&cubeMax, DirectX::XMVectorAdd(cubePos, cubeScale));

    // Sphere Data
    DirectX::XMVECTOR spherePos = DirectX::XMLoadFloat3(&other->mAttachedObject->Position);
    float sphereRadius = other->Radius;

    // Find closest point on cube to the sphere center
    DirectX::XMVECTOR closestPoint = DirectX::XMVectorClamp(spherePos, DirectX::XMLoadFloat3(&cubeMin), DirectX::XMLoadFloat3(&cubeMax));

    // Compute vector from sphere center to closest point
    DirectX::XMVECTOR toClosest = DirectX::XMVectorSubtract(spherePos, closestPoint);
    float distanceSquared = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(toClosest));

    // Check for overlap
    float radiusSquared = sphereRadius * sphereRadius;
    if (distanceSquared >= radiusSquared)
    {
        return false; // No collision
    }

    // Compute push direction and overlap
    float distance = std::sqrt(distanceSquared);
    minOverlap = sphereRadius - distance;

    pushDirection = (distance > 0.0f) ? DirectX::XMVector3Normalize(toClosest) : DirectX::XMVectorSet(1, 0, 0, 0); // Default push dir if inside

    return true;
}

// **Position correction (only modifies Cube, never the Sphere)**
void CubeCollider::ApplyPositionCorrectionFromSphere(const SphereCollider* other, DirectX::XMVECTOR pushDirection, float minOverlap) const
{
    // Normalize push direction
    pushDirection = DirectX::XMVector3Normalize(pushDirection);

    // Compute correction (move only the cube)
    DirectX::XMVECTOR correction = DirectX::XMVectorScale(pushDirection, minOverlap);

    // Apply correction to the cube position
    mAttachedObject->Position = DirectX::XMFLOAT3(
        mAttachedObject->Position.x + DirectX::XMVectorGetX(correction),
        mAttachedObject->Position.y + DirectX::XMVectorGetY(correction),
        mAttachedObject->Position.z + DirectX::XMVectorGetZ(correction)
    );
}

// **Velocity response (only modifies Cube, never the Sphere)**
void CubeCollider::ApplyVelocityResponseFromSphere(const SphereCollider* other, DirectX::XMVECTOR pushDirection) const
{
    // Normalize push direction
    pushDirection = DirectX::XMVector3Normalize(pushDirection);

    // Load velocities
    DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(&mAttachedObject->Velocity);
    DirectX::XMVECTOR sphereVelocity = DirectX::XMLoadFloat3(&other->mAttachedObject->Velocity);

    // Compute relative velocity
    DirectX::XMVECTOR relativeVelocity = DirectX::XMVectorSubtract(sphereVelocity, velocity);

    // Compute velocity along the normal
    float velocityAlongNormal = DirectX::XMVectorGetX(DirectX::XMVector3Dot(relativeVelocity, pushDirection));

    // If moving apart, no response needed
    if (velocityAlongNormal > 0.0f)
        return;

    // Get masses (assume mass of static objects is infinite, preventing them from changing velocity)
    float cubeMass = mAttachedObject->mMass;
    float sphereMass = other->mAttachedObject->mMass;

    // Avoid division by zero
    if (cubeMass <= 0.0f)
        return;

    // Coefficient of restitution (elasticity factor)
    float restitution = Elastic; // Use your defined elasticity value

    // Compute impulse scalar with mass consideration
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal / (1.0f / cubeMass + 1.0f / sphereMass);

    // Apply impulse to Cube only
    DirectX::XMVECTOR impulse = DirectX::XMVectorScale(pushDirection, impulseScalar / cubeMass);
    DirectX::XMStoreFloat3(&mAttachedObject->Velocity, DirectX::XMVectorAdd(velocity, impulse));
}
