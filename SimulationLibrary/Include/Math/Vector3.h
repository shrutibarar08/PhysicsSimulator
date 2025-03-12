#pragma once


namespace Phyx
{
    class Vector3D
    {

    public:
        Vector3D();
        Vector3D(const float x, const float y, const float z);
        ~Vector3D() = default;
        
        void Invert();
        void Normalize();
        void AddScaledVector(const Vector3D& vector, float scale);

        float Magnitude() const;
        float SquareMagnitude() const;

        void operator*=(const float value);
        Vector3D operator*(const float value) const;

        void operator+=(const Vector3D& vector);
        Vector3D operator+(const Vector3D& vector) const;

        void operator-=(const Vector3D& vector);
        Vector3D operator-(const Vector3D& vector) const;

        Vector3D ComponentProduct(const Vector3D& vector) const;
        void ComponentProductOnSelf(const Vector3D& vector);

        float ScalarProduct(const Vector3D& vector) const;
        float operator*(const Vector3D& vector) const;

        Vector3D VectorProduct(const Vector3D& vector) const;
        
        void operator%=(const Vector3D& vector);    // Update Vector to be a vector product of current values
        Vector3D operator%(const Vector3D& vector) const; // Return vector product

        void Reset();

    public:
        float x, y, z;

    private:
        float pad;
        float _start_x{ 0 }, _start_y{ 0 }, _start_z{ 0 };
    };
}

