#pragma once
#include <vector>
#include "SceneObject.hpp"
#include "../raymath/Transform.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/AABB.hpp"
#include "./Triangle.hpp"

class Mesh : public SceneObject
{
private:
  std::vector<Triangle *> triangles;
  AABB boundingBox;

public:
  Mesh();
  ~Mesh();

  void loadFromObj(std::string path);

  virtual void applyTransform() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
  void calculateBoundingBox();
};
