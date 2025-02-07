#include "pch.h"
#include "Line.h"

// Constructor
Line::Line(const Vector3& point, const Vector3& direction)
    : point(point), direction(direction.normalize()) {}

// Compute shortest distance
float Line::closestDistance(const Vector3& P_G) const {
    Vector3 V = P_G - point; // Vector from P_L to P_G
    Vector3 projection = direction * (V.dot(direction)); // Projection of V onto D_L
    Vector3 P_A = V - projection; // Perpendicular vector
    return P_A.length(); // Distance is magnitude of perpendicular vector
}

