#include "pch.h"
#include "Shapes/Line.h"
#include "Math/Vector3.h"

// Constructor
Line::Line(const Vector3& point, const Vector3& direction)
    : point(point), direction(direction.normalize()) {}

Vector3 Line::GetPoint(float m) const
{
    return point + (direction * m);
}

float Line::DistanceToPoint(const Vector3& q) const
{
    Vector3 pq = q - point;
    float t = pq.dot(direction);  // Project pq onto direction
    Vector3 closestPoint = point + (direction * t);
    return (q - closestPoint).length();
}
