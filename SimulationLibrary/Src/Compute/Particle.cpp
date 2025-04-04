#include "pch.h"
#include "Compute/Particle.h"

// Default constructor
Particle::Particle() : mMass(1.0f), mInverseMass(1.0f),
AccumulatedForce({ 0.0f, 0.0f, 0.0f })
{
}

void Particle::SetResting(bool value)
{
    mResting = value;
    mRestPosition = Position;
}

bool Particle::IsResting()
{
    if (mForcedUnRest)
    {
        mForcedUnRest = false;
        return false;
    }
    if (mResting) return true;
    if (mRestPosition.x == Position.x &&
        mRestPosition.y == Position.y &&
        mRestPosition.z == Position.z)
    {
        return true;
    }
    return false;
}

// Set mass and update inverse mass
void Particle::SetMass(float mass)
{
    if (mass > 0.0f)
    {
        mMass = mass;
        mInverseMass = 1.0f / mass;
    }
    else
    {
    	mMass = 0.0f;
        mInverseMass = 0.0f; // Infinite mass
    }
}

void Particle::ApplyForce(const DirectX::XMFLOAT3& force)
{
    SetResting(false);
    mForcedUnRest = true;
    AddForce(force);
}

// Add force to the accumulated forces
void Particle::AddForce(const DirectX::XMFLOAT3& force)
{
    AccumulatedForce.x += force.x;
    AccumulatedForce.y += force.y;
    AccumulatedForce.z += force.z;
}

// Reset accumulated forces
void Particle::ResetForce()
{
    AccumulatedForce = { 0.0f, 0.0f, 0.0f };
}

void Particle::ResetAccumulatedVelocity()
{
    AccumulatedVelocity = { 0.f, 0.f, 0.f };
}

// Set position
void Particle::SetPosition(const DirectX::XMFLOAT3& position)
{
    Position = position;
}

// Set velocity
void Particle::SetVelocity(const DirectX::XMFLOAT3& velocity)
{
    Velocity = velocity;
}

// Set acceleration
void Particle::SetAcceleration(const DirectX::XMFLOAT3& acceleration)
{
    Acceleration = acceleration;
}

// Getters
float Particle::GetMass() const { return mMass; }
float Particle::GetInverseMass() const { return mInverseMass; }
DirectX::XMFLOAT3 Particle::GetPosition() const { return Position; }
DirectX::XMFLOAT3 Particle::GetVelocity() const { return Velocity; }
DirectX::XMFLOAT3 Particle::GetAcceleration() const { return Acceleration; }
DirectX::XMFLOAT3 Particle::GetAccumulatedForce() const { return AccumulatedForce; }
