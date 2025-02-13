#pragma once
#include "Vector3.h"
#include "Line.h"
#include "Plane.h"

class Sphere {
public:
    Vector3 center;
    float radius;

    // Constructor
    Sphere(const Vector3& center, float radius);

    // Method to check collision with another sphere
    //bool CollideWith(const Sphere& other) const;

	//to check if a line intersects with the sphere
    //bool intersects(const Line& line) const;
    bool planeIntersects(const Plane& plane) const;
};



