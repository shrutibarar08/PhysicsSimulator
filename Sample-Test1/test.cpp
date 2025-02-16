#include "pch.h"
#include "Shapes/Sphere.h"
#include "Math/Vector3.h"
#include "Shapes/Line.h"
#include "Shapes/Plane.h"
#include <gtest/gtest.h>


TEST(SphereSphereCollision, NoIntersectionCentreAtOrigin) {
    Sphere sphereA(Vector3(0, 0, 0), 1);
    Sphere sphereB(Vector3(5, 0, 0), 1);
    EXPECT_FALSE(sphereA.IsInside(sphereB.GetPositionVector())) << "Spheres at (0,0,0) and (5,0,0) with radius 1 should not collide.";
}

TEST(SphereSphereCollision, NoIntersectionOffsetCentre) {
    Sphere sphereA(Vector3(3, 3, 3), 2);
    Sphere sphereB(Vector3(10, 10, 10), 2);
    EXPECT_FALSE(sphereA.IsInside(sphereB.GetPositionVector())) << "Spheres at (3,3,3) and (10,10,10) with radius 2 should not collide.";
}

TEST(SphereSphereCollision, OverlappingCentreAtOrigin) {
    Sphere sphereA(Vector3(0, 0, 0), 2);
    Sphere sphereB(Vector3(2, 0, 0), 2);
    EXPECT_TRUE(sphereA.IsInside(sphereB.GetPositionVector())) << "Spheres at (0,0,0) and (2,0,0) with radius 2 should overlap.";
}

TEST(SphereSphereCollision, OverlappingOffsetCentre) {
    Sphere sphereA(Vector3(5, 5, 5), 3);
    Sphere sphereB(Vector3(8, 5, 5), 3);
    EXPECT_TRUE(sphereA.IsInside(sphereB.GetPositionVector())) << "Spheres at (5,5,5) and (8,5,5) with radius 3 should overlap.";
}

TEST(SphereSphereCollision, FullyContainedCentreAtOrigin) {
    Sphere sphereA(Vector3(0, 0, 0), 5);
    Sphere sphereB(Vector3(1, 0, 0), 1);
    EXPECT_TRUE(sphereA.IsInside(sphereB.GetPositionVector())) << "A larger sphere fully containing a smaller one should collide.";
}

TEST(SphereSphereCollision, FullyContainedOffsetCentre) {
    Sphere sphereA(Vector3(6, 6, 6), 5);
    Sphere sphereB(Vector3(7, 6, 6), 2);
    EXPECT_TRUE(sphereA.IsInside(sphereB.GetPositionVector())) << "A larger sphere fully containing a smaller one should collide.";
}

TEST(PointLineDistance, ClosestPoint) {
    Line line(Vector3(0, 0, 0), Vector3(1, 1, 1));
    Vector3 point(2, 3, 4);
    float expectedDistance = std::sqrt(2);
    float actualDistance = line.DistanceToPoint(point);
    EXPECT_NEAR(actualDistance, expectedDistance, 0.01);
}

TEST(PointLineDistance, PointOnLine) {
    Line line(Vector3(0, 0, 0), Vector3(1, 2, 3));
    Vector3 point(3, 6, 9);
    EXPECT_NEAR(line.DistanceToPoint(point), 0.0f, 0.01);
}

TEST(PointLineDistance, VerticalCase) {
    Line line(Vector3(2, 0, 0), Vector3(0, 1, 0));
    Vector3 point(4, 5, 3);
    float expectedDistance = 3.61;
    float actualDistance = line.DistanceToPoint(point);
    EXPECT_NEAR(actualDistance, expectedDistance, 0.01);
}

TEST(SphereLineIntersection, NoIntersection) {
    Line line(Vector3(5, 5, 5), Vector3(1, 0, 0));
    Sphere sphere(Vector3(0, 0, 0), 3);
    EXPECT_FALSE(sphere.Intersects(&line));
}

TEST(SphereLineIntersection, PassesThroughSphere) {
    Line line(Vector3(10, 0, 0), Vector3(-1, 0, 0));
    Sphere sphere(Vector3(10, 0, 0), 5);
    EXPECT_TRUE(sphere.Intersects(&line));
}

TEST(SphereLineIntersection, LineStartsInsideSphere) {
    Line line(Vector3(3, 2, 2), Vector3(1, 0, 0));
    Sphere sphere(Vector3(2, 2, 2), 5);
    EXPECT_TRUE(sphere.Intersects(&line));
}

TEST(SphereLineIntersection, LinePassesThroughCenter) {
    Line line(Vector3(-5, 0, 0), Vector3(1, 0, 0));
    Sphere sphere(Vector3(0, 0, 0), 3);
    EXPECT_TRUE(sphere.Intersects(&line));
}

TEST(PointPlaneDistance, PointAbovePlane) {
    Plane plane(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Vector3 point(2, 3, 5);
    EXPECT_NEAR(plane.GetPointDistance(point), 5.0, 0.01);
}

TEST(PointPlaneDistance, PointBelowPlane) {
    Plane plane(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Vector3 point(2, 3, -4);
    EXPECT_NEAR(plane.GetPointDistance(point), 4.0, 0.01);
}

TEST(PointPlaneDistance, PointOnPlane) {
    Plane plane(Vector3(1, 1, 1), Vector3(1, 1, 1).normalize());
    Vector3 point(0, 2, 1);
    EXPECT_NEAR(plane.IsInside(point), 0.0, 0.01);
}

TEST(SpherePlaneCollision, SphereAbovePlane_NoCollision) {
    Plane plane(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Sphere sphere(Vector3(0, 0, 5), 3);
    EXPECT_FALSE(sphere.Intersects(reinterpret_cast<Line*>(&plane)));
}

TEST(SpherePlaneCollision, SphereTouchesPlane) {
    Plane plane(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Sphere sphere(Vector3(0, 0, 3), 3);
    EXPECT_TRUE(sphere.Intersects(reinterpret_cast<Line*>(&plane)));
}

TEST(SpherePlaneCollision, SphereIntersectsPlane) {
    Plane plane(Vector3(0, 0, 0), Vector3(0, 0, 1));
    Sphere sphere(Vector3(0, 0, 2), 3);
    EXPECT_TRUE(sphere.Intersects(reinterpret_cast<Line*>(&plane)));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
