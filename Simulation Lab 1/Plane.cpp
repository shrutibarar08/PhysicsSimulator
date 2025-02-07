#include "pch.h"
#include "Plane.h"
#include <cmath>
Plane::Plane(const Vector3& point, const Vector3& normal)
    : point(point), normal(normal.normalize()) {}

// Compute shortest distance from a point to the plane
float Plane::closestDistance(const Vector3& P_G) const {
    // Vector from plane point to general point
    Vector3 V = P_G - point;  
    // Perpendicular distance using dot product
    return std::fabs(V.dot(normal));  
}
