#pragma once
#include "Math/Vector3.h"
#include "Shapes/Collider.h"


class Sphere final: public Collider
{
public:
	Sphere(const Vector3& position, float radius);

	bool IsInside(const Vector3& other) const override;
	bool Intersects(const Line* other) const override;
	float GetPointDistance(const Vector3& point) const override;

private:
    float mRadius;
};
