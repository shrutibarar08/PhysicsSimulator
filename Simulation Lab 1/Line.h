#pragma once
#include "Vector3.h"

class Line {
public:
    Vector3 point;
    Vector3 direction;

    Line(const Vector3& point, const Vector3& direction); // Constructor


    float closestDistance(const Vector3& P_G) const; // Method
};

