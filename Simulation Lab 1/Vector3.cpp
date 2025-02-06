#include "pch.h"
#include "Vector3.h"

// Constructor
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// Vector subtraction
Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

// Calculate the squared length
float Vector3::LengthSquared() const {
    return x * x + y * y + z * z;
}

