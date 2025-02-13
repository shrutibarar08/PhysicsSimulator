#include "pch.h"
#include "Plane.h"
#include <cmath>

// Constructor
Plane::Plane(const Vector3& point, const Vector3& normal) : point(point), normal(normal) {}

// Method to calculate the closest distance from a point to the plane
float Plane::closestDistance(const Vector3& P_G) const {
    Vector3 V = P_G - point;
    return std::fabs(V.x * normal.x + V.y * normal.y + V.z * normal.z);
}

// Method to check if a sphere intersects with the plane
bool Plane::planeintersects(const Sphere& sphere) const {
    float distance = closestDistance(sphere.center);
    return distance <= sphere.radius;
}




