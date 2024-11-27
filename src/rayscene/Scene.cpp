#include <iostream>
#include "Scene.hpp"
#include "Intersection.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
  for (int i = 0; i < objects.size(); ++i)
  {
    delete objects[i];
  }

  for (int i = 0; i < lights.size(); ++i)
  {
    delete lights[i];
  }
}

void Scene::add(SceneObject *object)
{
  objects.push_back(object);
}

void Scene::addLight(Light *light)
{
  lights.push_back(light);
}

void Scene::prepare()
{
  for (int i = 0; i < objects.size(); ++i)
  {
    objects[i]->applyTransform();
  }
}

std::vector<Light *> Scene::getLights()
{
  return lights;
}

bool Scene::closestIntersection(Ray &r, Intersection &closest, CullingType culling)
{
  Intersection intersection;

  double closestDistance = -1;
  Intersection closestInter;
  for (int i = 0; i < objects.size(); ++i)
  {
    if (objects[i]->intersects(r, intersection, culling))
    {

      intersection.Distance = (intersection.Position - r.GetPosition()).length();
      if (closestDistance < 0 || intersection.Distance < closestDistance)
      {
        closestDistance = intersection.Distance;
        closestInter = intersection;
      }
    }
  }
  closest = closestInter;
  return (closestDistance > -1);
}

Color Scene::raycast(Ray &r, Ray &camera, int castCount, int maxCastCount)
{

  Color pixel;

  Intersection intersection;

  if (closestIntersection(r, intersection, CULLING_FRONT))
  {
    // Add the view-ray for convenience (the direction is normalised in the constructor)
    intersection.View = (camera.GetPosition() - intersection.Position).normalize();

    if (intersection.Mat != NULL)
    {
      pixel = pixel + (intersection.Mat)->render(r, camera, &intersection, this);

      // Reflect
      if (castCount < maxCastCount & intersection.Mat->cReflection > 0)
      {
        Vector3 reflectDir = r.GetDirection().reflect(intersection.Normal);
        Vector3 origin = intersection.Position + (reflectDir * COMPARE_ERROR_CONSTANT);
        Ray reflectRay(origin, reflectDir);

        pixel = pixel + raycast(reflectRay, camera, castCount + 1, maxCastCount) * intersection.Mat->cReflection;
      }
    }
  }

  return pixel;
}