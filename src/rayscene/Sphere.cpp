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
}

// void Sphere::countPrimes() {
//  int prime_counter = 0;
//  for(int n = 2 ; n<1000 ; n++)
//   {
//     int count = 0;
//     for (int i = 2; i <= i/2; i++)
//     {
//       if(n%i == 0) {
//         count++;
//       }
//       if(count == 0)
//       {
//         prime_counter++;
//       }  
//     }
//   }
// }

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vector from ray origin to center of sphere
  Vector3 OC = center - r.GetPosition();

  // Project OC onto the ray
  double tca = OC.dot(r.GetDirection());
  
  // If the projection is negative, the sphere is behind the ray origin
  if (tca < 0)
  {
    return false;
  }

  // Calculate the squared distance from the sphere center to the projection
  double d2 = OC.dot(OC) - tca * tca;
  double radius2 = radius * radius;

  // If the distance is greater than the radius, there is no intersection
  if (d2 > radius2)
  {
    return false;
  }

  // Calculate the distance from the projection to the intersection points
  double thc = sqrt(radius2 - d2);

  // Calculate the intersection point along the ray
  double t = tca - thc;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  // Pre-calculate some useful values for rendering
  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();
  // Junk function!!
  // countPrimes();

  return true;
}
