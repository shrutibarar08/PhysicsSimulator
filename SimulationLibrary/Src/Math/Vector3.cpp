#include "pch.h"
#include "Math/Vector3.h"

#include <cmath>
#include <iostream>


Physics::Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f), pad(0.0f) {}

Physics::Vector3D::Vector3D(const float x, const float y, const float z) : x(x), y(y), z(z), pad(0.0f) {}

void Physics::Vector3D::Invert()
{
    x = -x;
    y = -y;
    z = -z;
}

void Physics::Vector3D::Normalize()
{
    float magnitude = Magnitude();
    if (magnitude > 0.0f)
    {
        *this *= (1.0f / magnitude);
    }
}

void Physics::Vector3D::AddScaledVector(const Vector3D& vector, float scale)
{
    x += vector.x * scale;
    y += vector.y * scale;
    z += vector.z * scale;
}

float Physics::Vector3D::Magnitude() const
{
    return std::sqrt(SquareMagnitude());
}

float Physics::Vector3D::SquareMagnitude() const
{
    return x * x + y * y + z * z;
}

void Physics::Vector3D::operator*=(const float value)
{
    x *= value;
    y *= value;
    z *= value;
}

Physics::Vector3D Physics::Vector3D::operator*(const float value) const
{
    return Vector3D(x * value, y * value, z * value);
}

void Physics::Vector3D::operator+=(const Vector3D& vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
}

Physics::Vector3D Physics::Vector3D::operator+(const Vector3D& vector) const
{
    return Vector3D(x + vector.x, y + vector.y, z + vector.z);
}

void Physics::Vector3D::operator-=(const Vector3D& vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
}

Physics::Vector3D Physics::Vector3D::operator-(const Vector3D& vector) const
{
    return Vector3D(x - vector.x, y - vector.y, z - vector.z);
}

Physics::Vector3D Physics::Vector3D::ComponentProduct(const Vector3D& vector) const
{
    return Vector3D(x * vector.x, y * vector.y, z * vector.z);
}

void Physics::Vector3D::ComponentProductOnSelf(const Vector3D& vector)
{
    x *= vector.x;
    y *= vector.y;
    z *= vector.z;
}

float Physics::Vector3D::ScalarProduct(const Vector3D& vector) const
{
    return x * vector.x + y * vector.y + z * vector.z;
}

float Physics::Vector3D::operator*(const Vector3D& vector) const
{
    return ScalarProduct(vector);
}

Physics::Vector3D Physics::Vector3D::VectorProduct(const Vector3D& vector) const
{
    return Vector3D(
        y * vector.z - z * vector.y,
        z * vector.x - x * vector.z,
        x * vector.y - y * vector.x
    );
}

void Physics::Vector3D::operator%=(const Vector3D& vector)
{
    *this = VectorProduct(vector);
}

Physics::Vector3D Physics::Vector3D::operator%(const Vector3D& vector) const
{
    return VectorProduct(vector);
}
