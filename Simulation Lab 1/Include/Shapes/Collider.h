#pragma once

#include "Math/Vector3.h"
#include "Shapes/Line.h"

/*
 *	Base Class for Shapes
 */

class Collider
{
public:
	Collider(const Vector3& position): mPosition(position)
	{}

	virtual ~Collider() = default;

	Collider(const Collider&) = default;
	Collider& operator=(const Collider&) = default;
	Collider(Collider&&) = default;
	Collider& operator=(Collider&&) = default;

	//~ Base Methods
	virtual bool IsInside(const Vector3& other) const = 0;
	virtual bool Intersects(const Line* other) const = 0;
	virtual float GetPointDistance(const Vector3& point) const = 0;

	//~ Getters
	[[nodiscard]] Vector3 GetPositionVector() const;

protected:
	Vector3 mPosition;
};
