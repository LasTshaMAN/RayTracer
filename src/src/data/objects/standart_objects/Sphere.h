#ifndef __SPHERE__
#define __SPHERE__

#include "jsoncons\json.hpp"

#include "StandartObject.h"
#include "..\..\materials\Material.h"

class Sphere : public StandartObject {
public:
    Sphere(const jsoncons::json& standartObject);
    ~Sphere();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    float radius;
    const Material * material;
};

#endif