#pragma once
#include "Particle.h"
#include <DirectXMath.h>
#include <cmath>


namespace Phyx
{
    enum class EIntegration: uint8_t
    {
        euler_integration,
        semi_implicit_euler_integration
    };
}


class IntegrationInterface
{
public:
    virtual void Integrate(Particle& particle, float deltaTime) = 0;
    virtual ~IntegrationInterface() = default;
};

class EulerIntegration : public IntegrationInterface
{
public:
    void Integrate(Particle& particle, float deltaTime) override
    {
        using namespace DirectX;

        if (particle.GetInverseMass() <= 0.0f) return; // Avoid division by zero for static objects

        // Load current values
        XMVECTOR position = XMLoadFloat3(&particle.Position);
        XMVECTOR velocity = XMLoadFloat3(&particle.Velocity);
        XMVECTOR accVelocity = XMLoadFloat3(&particle.AccumulatedVelocity);

        XMVECTOR accumulatedForce = XMLoadFloat3(&particle.AccumulatedForce);
        XMVECTOR defaultAcceleration = XMLoadFloat3(&particle.Acceleration);
        float inverseMass = particle.GetInverseMass();
        float damping = particle.mDamping;  // Retrieve damping factor
        
        // Compute total acceleration
        XMVECTOR forceAcceleration = XMVectorScale(accumulatedForce, inverseMass);
        XMVECTOR totalAcceleration = XMVectorAdd(defaultAcceleration, forceAcceleration);

        // Euler Integration: x' = x + v*dt, v' = v + a*dt
        auto totalVelocity = XMVectorAdd(velocity, accVelocity);
        position = XMVectorAdd(position, XMVectorScale(totalVelocity, deltaTime));
        velocity = XMVectorAdd(velocity, XMVectorScale(totalAcceleration, deltaTime));

        // Apply damping to velocity (exponential decay)
        velocity = XMVectorScale(velocity, std::pow(damping, deltaTime));

        // Store results
        XMStoreFloat3(&particle.Position, position);
        XMStoreFloat3(&particle.Velocity, velocity);

        // Reset accumulated force after applying
        particle.ResetForce();
        particle.ResetAccumulatedVelocity();
    }
};

class SemiImplicitEulerIntegration : public IntegrationInterface
{
public:
    void Integrate(Particle& particle, float deltaTime) override
    {
        using namespace DirectX;

        if (particle.GetInverseMass() <= 0.0f) return; // Avoid division by zero for static objects

        // Load current values
        XMVECTOR position = XMLoadFloat3(&particle.Position);
        XMVECTOR velocity = XMLoadFloat3(&particle.Velocity);
        XMVECTOR accVelocity = XMLoadFloat3(&particle.AccumulatedVelocity);
        XMVECTOR accumulatedForce = XMLoadFloat3(&particle.AccumulatedForce);
        XMVECTOR defaultAcceleration = XMLoadFloat3(&particle.Acceleration);
        float inverseMass = particle.GetInverseMass();
        float damping = particle.mDamping;  // Retrieve damping factor

        // Compute total acceleration
        XMVECTOR forceAcceleration = XMVectorScale(accumulatedForce, inverseMass);
        XMVECTOR totalAcceleration = XMVectorAdd(defaultAcceleration, forceAcceleration);

        // Semi-Implicit Euler: v' = v + a*dt, then apply damping
        velocity = XMVectorAdd(velocity, XMVectorScale(totalAcceleration, deltaTime));
        velocity = XMVectorScale(velocity, std::pow(damping, deltaTime)); // Apply damping

        // Update position using new velocity
        auto totalVelocity = XMVectorAdd(velocity, accVelocity);
        position = XMVectorAdd(position, XMVectorScale(totalVelocity, deltaTime));

        // Store results
        XMStoreFloat3(&particle.Velocity, velocity);
        XMStoreFloat3(&particle.Position, position);

        // Reset accumulated force after applying
        particle.ResetForce();
        particle.ResetAccumulatedVelocity();
    }
};
