#pragma once

#include "XParticleEffectInterface.h"


class XPEGravity: public IParticleEffectInterface
{
public:
	XPEGravity(const Phyx::Vector3D& gravity);
	virtual void UpdateForce(XParticle* particle, float duration) override;

private:
	Phyx::Vector3D m_Gravity;
};


