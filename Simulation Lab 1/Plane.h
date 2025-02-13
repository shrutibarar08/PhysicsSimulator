#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Sphere.h"

class Plane {
public:
    Vector3 point;
    Vector3 normal;

    Plane(const Vector3& point, const Vector3& normal);

    float closestDistance(const Vector3& P_G) const;
    bool planeintersects(const Sphere& sphere) const;
};


