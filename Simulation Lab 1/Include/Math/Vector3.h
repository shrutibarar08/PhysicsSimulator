#pragma once

class Vector3 {
    public:
        float x, y, z;

        // Constructor
        Vector3();
        Vector3(float x, float y, float z);
        float lengthSquared() const;

        // Vector subtraction
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator*(float scalar) const;
        float dot(const Vector3& other) const;
        float length() const;
        Vector3 normalize() const;

        // Print
        void print() const;
};

