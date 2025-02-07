#pragma once
#include "pch.h"
#include "Vector3.h"

class Plane {
public:
    Vector3 point;   // A point on the plane
    Vector3 normal;  // Normal vector of the plane

    Plane(const Vector3& point, const Vector3& normal);
    float closestDistance(const Vector3& P_G) const;
};


