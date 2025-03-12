#include "pch.h"
#include "Physics/ParticleEffects/XPEGravity.h"


XPEGravity::XPEGravity(const Phyx::Vector3D& gravity): m_Gravity(gravity)
{}

void XPEGravity::UpdateForce(XParticle* particle, float duration)
{
	if (!particle->IsFiniteMass()) return;

	particle->AddForce(m_Gravity * particle->GetMass());
}
