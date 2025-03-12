#include "pch.h"
#include "Physics/Particle.h"


void XParticle::Integrate(float duration)
{
	assert(duration > 0.0);

	//~ Update Position
	m_Position.AddScaledVector(m_Velocity, duration);

	//~ Get Acceleration from force
	Phyx::Vector3D acceleration = m_Acceleration;
	acceleration.AddScaledVector(m_ForceAccumulation, m_InverseMass);

	//~ Update Velocity
	m_Velocity.AddScaledVector(acceleration, duration);

	//~ Apply Drag
	m_Velocity *= std::powf(m_LinearDrag, duration);

	//~ Clear accumulated forces
	m_ForceAccumulation.Reset();
}

void XParticle::AddForce(const Phyx::Vector3D& force)
{
	m_ForceAccumulation += force;
}

Phyx::Vector3D XParticle::GetPosition() const
{
	return m_Position;
}

Phyx::Vector3D XParticle::GetVelocity() const
{
	return m_Velocity;
}

Phyx::Vector3D XParticle::GetAcceleration() const
{
	return m_Acceleration;
}

Phyx::Vector3D XParticle::GetForceAccumulation() const
{
	return m_ForceAccumulation;
}

float XParticle::GetLinearDrag() const
{
	return m_LinearDrag;
}

float XParticle::GetInverseMass() const
{
	return m_InverseMass;
}

float XParticle::GetMass() const
{
	return m_Mass;
}

bool XParticle::IsFiniteMass() const
{
	return m_bFiniteMass;
}

void XParticle::SetFiniteMass(bool status)
{
	m_bFiniteMass = status;
}

void XParticle::SetPosition(const Phyx::Vector3D& position)
{
	m_Position = position;
}

void XParticle::SetVelocity(const Phyx::Vector3D& velocity)
{
	m_Velocity = velocity;
}

void XParticle::SetAcceleration(const Phyx::Vector3D& acceleration)
{
	m_Acceleration = acceleration;
}

void XParticle::SetForceAccumulation(const Phyx::Vector3D& forceAccumulated)
{
	m_ForceAccumulation = forceAccumulated;
}

void XParticle::SetLinearDrag(float linearDrag)
{
	m_LinearDrag = linearDrag;
}

void XParticle::SetInverseMass(float inverseMass)
{
	m_InverseMass = inverseMass;
}

void XParticle::SetMass(float mass)
{
	m_Mass = mass;
}
