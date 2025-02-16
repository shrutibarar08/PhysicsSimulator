#pragma once
#include "Math/Vector3.h"
#include "Collider.h"


class Plane: public Collider
{
public:
    Plane(const Vector3& position, const Vector3& normal);

    bool IsInside(const Vector3& point) const override;
    bool Intersects(const Line* other) const override;
    float GetPointDistance(const Vector3& point) const override;

private:
    Vector3 mNormal;
};

