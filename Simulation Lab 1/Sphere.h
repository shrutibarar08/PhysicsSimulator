#pragma once
#include "Vector3.h"

class Sphere {
public:
    Vector3 center;
    float radius;

    // Constructor
    Sphere(const Vector3& center, float radius);

    // Method to check collision with another sphere
    bool CollideWith(const Sphere& other) const;
};



