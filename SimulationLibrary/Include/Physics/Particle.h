#pragma once
#include "Core.h"


class XParticle
{
public:

	void Integrate(float duration);
	void AddForce(const Phyx::Vector3D& force);

	Phyx::Vector3D GetPosition() const;
	Phyx::Vector3D GetVelocity() const;
	Phyx::Vector3D GetAcceleration() const;
	Phyx::Vector3D GetForceAccumulation() const;

	float GetLinearDrag() const;
	float GetInverseMass() const;
	float GetMass() const;

	bool IsFiniteMass() const;
	void SetFiniteMass(bool status);

	void SetPosition(const Phyx::Vector3D& position);
	void SetVelocity(const Phyx::Vector3D& velocity);
	void SetAcceleration(const Phyx::Vector3D& acceleration);
	void SetForceAccumulation(const Phyx::Vector3D& forceAccumulated);
	void SetLinearDrag(float linearDrag);
	void SetInverseMass(float inverseMass);
	void SetMass(float mass);

private:
	bool m_bFiniteMass{ false };

	Phyx::Vector3D m_Position;
	Phyx::Vector3D m_Velocity;
	Phyx::Vector3D m_Acceleration;
	Phyx::Vector3D m_ForceAccumulation;

	float m_LinearDrag;
	float m_InverseMass;
	float m_Mass;
};
