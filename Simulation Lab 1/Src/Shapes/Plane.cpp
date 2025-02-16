#include "pch.h"
#include "Shapes/Plane.h"
#include "Math/Vector3.h"
#include <cmath>

// Constructor
Plane::Plane(const Vector3& position, const Vector3& normal)
: Collider(position), mNormal(normal.normalize())
{}

bool Plane::IsInside(const Vector3 & point) const
{
    float d = mNormal.dot(mPosition); // Compute D dynamically
    return fabs(mNormal.dot(point) - d) < 1e-5;
}

bool Plane::Intersects(const Line* other) const
{
    float d = mNormal.dot(mPosition);
    float d_norm = mNormal.dot(other->direction);

	if (fabs(d_norm) < 1e-5) return false;

	float t = (d - mNormal.dot(other->point)) / d_norm;
    return t >= 0;
}

float Plane::GetPointDistance(const Vector3& point) const
{
    return fabs(mNormal.dot(point) - mNormal.dot(mPosition)) / mNormal.length();
}

