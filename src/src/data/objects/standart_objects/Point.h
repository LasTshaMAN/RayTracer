#ifndef __POINT__
#define __POINT__

#include "jsoncons\json.hpp"

#include "StandartObject.h"

class Point : public StandartObject {
public:
    Point(const jsoncons::json& standartObject);
    ~Point();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    void prepareSamples();

private:
    glm::vec3 point;
};

#endif