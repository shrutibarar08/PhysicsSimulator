#pragma once
#include "Physics/ParticleEffects/XParticleEffectInterface.h"


class XPEDrag: public IParticleEffectInterface
{
public:
	XPEDrag(float k1, float k2);
	virtual void UpdateForce(XParticle* particle, float duration) override;

private:
	float m_K1;
	float m_K2;
};
