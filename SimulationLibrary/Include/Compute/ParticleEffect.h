#pragma once

#include "Particle.h"

class ParticleEffectInterface
{
public:
    ParticleEffectInterface() = default;
    virtual ~ParticleEffectInterface() = default;

	virtual void ApplyEffect(Particle* particle, float deltaTime) = 0;
};

class GravityFreeFallEffect final : public ParticleEffectInterface
{
public:
    explicit GravityFreeFallEffect(const DirectX::XMFLOAT3& gravity = { 0.0f, -9.81f, 0.0f })
        : mGravity(gravity) {}

    void ApplyEffect(Particle* particle, float deltaTime) override;

private:
    float mTotalTime{ 0.0f };
    DirectX::XMFLOAT3 mGravity{ 0, -9.81f, 0 };
    DirectX::XMFLOAT3 mLocalVelocity{ 0, 0.f, 0 };
};

class GravityEffect final: public ParticleEffectInterface
{
public:
    explicit GravityEffect(const DirectX::XMFLOAT3& gravity = { 0.0f, -9.81f, 0.0f })
        : mGravity(gravity) {}

    void ApplyEffect(Particle* particle, float deltaTime) override;

private:
    DirectX::XMFLOAT3 mGravity{ 0, -9.81f, 0 };
    float mRestThreshold{ 3.f };
    float mRestTime{ 1.f };
    bool mObjectAtRest{ false };
    float mAccumulatedRestTime{ 0.0f };
};
