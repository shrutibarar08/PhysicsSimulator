#pragma once

#include "Particle.h"

class ParticleEffectInterface
{
public:
	virtual void ApplyEffect(Particle& particle, float deltaTime) = 0;
	~ParticleEffectInterface() = default;
};

class GravityEffect final : public ParticleEffectInterface
{
public:
    explicit GravityEffect(const DirectX::XMFLOAT3& gravity = { 0.0f, -9.81f, 0.0f })
        : mGravity(gravity) {}

    void ApplyEffect(Particle& particle, float deltaTime) override;

private:
    float mTotalTime{ 0.0f };
    DirectX::XMFLOAT3 mGravity{ 0, -9.81f, 0 };
    DirectX::XMFLOAT3 mLocalVelocity{ 0, 0.f, 0 };
};
