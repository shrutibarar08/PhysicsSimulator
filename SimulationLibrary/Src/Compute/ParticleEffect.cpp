#include "pch.h"
#include "Compute/ParticleEffect.h"

#include <iostream>


void GravityFreeFallEffect::ApplyEffect(Particle* particle, float deltaTime)
{
    using namespace DirectX;

    // Convert to XMVECTOR for calculations
    XMVECTOR accelerationVec = XMLoadFloat3(&mGravity);
    XMVECTOR velocityVec = XMLoadFloat3(&mLocalVelocity);

    // Integrate velocity: v = v + a * dt
    velocityVec = XMVectorAdd(velocityVec, XMVectorScale(accelerationVec, deltaTime));

    // Store locally so it persists across frames
    XMStoreFloat3(&mLocalVelocity, velocityVec);

    // Apply accumulated velocity change to particle
    XMVECTOR particleVelocity = XMLoadFloat3(&particle->AccumulatedVelocity);
    particleVelocity = XMVectorAdd(particleVelocity, velocityVec);
    XMStoreFloat3(&particle->AccumulatedVelocity, particleVelocity);
}

void GravityEffect::ApplyEffect(Particle* particle, float deltaTime)
{
    if (particle->IsResting())
    {
        std::cout << "Gravity Effect is OFF!\n";
    }else
    {
        std::cout << "Applying Gravity Effect!\n";
        particle->AddForce(mGravity);
    }
}
