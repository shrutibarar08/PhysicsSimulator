#include "pch.h"
#include "Vector3.h"
#include <cmath>
#include <iostream>

// Default constructor
Vector3::Vector3() : x(0), y(0), z(0) {}

// Parameterized constructor
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// Method to calculate the squared length of the vector
float Vector3::lengthSquared() const {
    return x * x + y * y + z * z;
}

// Subtraction
Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

// Addition
Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

// Scalar multiplication
Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Dot product
float Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

// Length (magnitude)
float Vector3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Normalize
Vector3 Vector3::normalize() const {
    float len = length();
    return (len > 0) ? *this * (1.0f / len) : Vector3(0, 0, 0);
}

// Print
void Vector3::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")";
}



