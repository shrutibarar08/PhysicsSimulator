#include "pch.h"
#include "Shapes/Sphere.h"
#include <cmath>

// Constructor
Sphere::Sphere(const Vector3& position, float radius)
: Collider(position), mRadius(radius)
{}

bool Sphere::IsInside(const Vector3& other) const
{
    return (other - mPosition).lengthSquared() <= mRadius * mRadius;
}

bool Sphere::Intersects(const Line* other) const
{
    return other->DistanceToPoint(mPosition) <= mRadius;
}

float Sphere::GetPointDistance(const Vector3& point) const
{
	float x = mPosition.x - point.x;
	float y = mPosition.y - point.y;
	float z = mPosition.z - point.z;
	return std::sqrt(x * x + y * y + z * z) - mRadius;
}
