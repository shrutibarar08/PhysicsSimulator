#include "pch.h"
#include "Sphere.h"

// Constructor
Sphere::Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}

bool Sphere::planeIntersects(const Plane& plane) const
{
    float distance = plane.closestDistance(center);
    return distance <= radius;
}

bool Sphere::planeIntersects(const Plane& plane) const
{
    float distance = plane.closestDistance(center); 
    return distance <= radius;
}

// Check collision with another sphere
//bool Sphere::CollideWith(const Sphere& other) const{
//    // Calculate the squared distance between centers
//    //float distanceSquared = (center - other.center).LengthSquared();
//
//    // Calculate the squared sum of radii
//    float distanceSquared = (other.center - center).lengthSquared();
//    float radiusSum = radius + other.radius;
//
//    // Check if the squared distance is less than or equal to squared radii sum
//    //return distanceSquared <= radiiSumSquared;
//    return distanceSquared <= (radiusSum * radiusSum);
//}

