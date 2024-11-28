#pragma once
#include "../raymath/Ray.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "../raymath/Transform.hpp"
#include "../raymath/AABB.hpp"

enum CullingType
{
  CULLING_FRONT,
  CULLING_BACK,
  CULLING_BOTH
};

class SceneObject
{
protected:
  AABB boundingBox;

public:
  std::string name = "";
  Material *material = NULL;
  Transform transform;

  SceneObject();
  virtual ~SceneObject();

  virtual void applyTransform();
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling);
  virtual void calculateBoundingBox() = 0;
  AABB getBoundingBox() const { return boundingBox; }
};