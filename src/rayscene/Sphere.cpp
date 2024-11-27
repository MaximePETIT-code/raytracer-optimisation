#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
#ifdef ENABLE_BOUNDING_BOX
  calculateBoundingBox();
#endif
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
#ifdef ENABLE_BOUNDING_BOX
  // Vérifie l'AABB pour exclure l'objet si le rayon ne l'intersecte pas
  if (!boundingBox.intersects(r))
  {
    return false;
  }
#endif
  // Pré-calculer les informations du rayon
  const Vector3 &rayOrigin = r.GetPosition();
  const Vector3 &rayDirection = r.GetDirection();

  // Vector from ray origin to center of sphere
  Vector3 OC = center - rayOrigin;

  // Project OC onto the ray
  Vector3 OP = OC.projectOn(rayDirection);

  // If the OP vector is pointing in the opposite direction of the ray
  // ... then it is behind the ray origin, ignore the object
  if (OP.dot(rayDirection) <= 0)
  {
    return false;
  }

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 P = rayOrigin + OP;

  // Is the length of CP greater than the radius of the circle ? If yes, no intersection!
  Vector3 CP = P - center;
  double distanceSquared = CP.lengthSquared();
  double radiusSquared = radius * radius;
  if (distanceSquared > radiusSquared)
  {
    return false;
  }

  // Calculer la distance du point d'intersection au long du rayon
  double a = sqrt(radiusSquared - distanceSquared);
  double t = OP.length() - a;

  // Calculer le point exact d'intersection
  Vector3 P1 = rayOrigin + rayDirection * t;

  // Préparer les informations d'intersection
  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();

  return true;
}

void Sphere::calculateBoundingBox()
{
  Vector3 minPoint = center - Vector3(radius, radius, radius);
  Vector3 maxPoint = center + Vector3(radius, radius, radius);
  boundingBox = AABB(minPoint, maxPoint);
}
