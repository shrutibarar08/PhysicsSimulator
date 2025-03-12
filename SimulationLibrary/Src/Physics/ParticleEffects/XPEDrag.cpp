#include "pch.h"
#include "Physics/ParticleEffects/XPEDrag.h"


XPEDrag::XPEDrag(float k1, float k2)
	: m_K1(k1), m_K2(k2) 
{}

void XPEDrag::UpdateForce(XParticle* particle, float duration)
{
	Phyx::Vector3D force = particle->GetVelocity();
	float dragCf = force.Magnitude();

	dragCf = m_K1 * dragCf + m_K2 * dragCf * dragCf;

	force.Normalize();
	force *= -dragCf;
	particle->AddForce(force);
}
