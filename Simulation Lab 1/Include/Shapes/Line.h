#pragma once

#include "Math/Vector3.h"


class Line {
public:
    Vector3 point;      // A point on the line (PL)
    Vector3 direction;  // Direction of the line (DL), should be unit length

    // Constructor
    Line(const Vector3& p, const Vector3& d);
    // Get a point on the line at parameter m (PL + mDL)
    Vector3 GetPoint(float m) const;
    // Compute the shortest distance from a given point to the line
    float DistanceToPoint(const Vector3& q) const;
};
