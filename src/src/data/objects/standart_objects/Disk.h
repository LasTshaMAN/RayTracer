#ifndef __DISK__
#define __DISK__

#include "jsoncons\json.hpp"

#include "StandartObject.h"
#include "..\..\materials\Material.h"

class Disk : public StandartObject {
public:
    Disk(const jsoncons::json& standartObject);
    ~Disk();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    void prepareSamples();

private:
    float radius;
    const Material * material;
};

#endif