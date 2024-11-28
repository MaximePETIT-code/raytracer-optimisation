#include <iostream>
#include <cmath>
#include <limits>
#include "Plane.hpp"
#include "../raymath/Vector3.hpp"

Plane::Plane(Vector3 p, Vector3 n) : point(p), normal(n)
{
#ifdef ENABLE_BOUNDING_BOX
    calculateBoundingBox();
#endif
}

Plane::~Plane()
{
}

void Plane::calculateBoundingBox()
{
    const double INF = std::numeric_limits<double>::max();
    Vector3 minPoint(-INF, -INF, -INF);
    Vector3 maxPoint(INF, INF, INF);
    
    if (std::abs(normal.x) > 0.99) {
        minPoint.x = point.x - 0.001;
        maxPoint.x = point.x + 0.001;
    }
    if (std::abs(normal.y) > 0.99) {
        minPoint.y = point.y - 0.001;
        maxPoint.y = point.y + 0.001;
    }
    if (std::abs(normal.z) > 0.99) {
        minPoint.z = point.z - 0.001;
        maxPoint.z = point.z + 0.001;
    }
    
    boundingBox = AABB(minPoint, maxPoint);
}

bool Plane::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
#ifdef ENABLE_BOUNDING_BOX
    if (!boundingBox.intersects(r))
    {
        return false;
    }
#endif

    float denom = r.GetDirection().dot(normal);

    if (denom > -0.000001)
    {
        return false;
    }

    float numer = (point - r.GetPosition()).dot(normal);
    float t = numer / denom;

    intersection.Position = r.GetPosition() + (r.GetDirection() * t);
    intersection.Normal = normal;
    intersection.Mat = this->material;

    return true;
}