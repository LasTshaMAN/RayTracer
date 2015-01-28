#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "jsoncons\json.hpp"

#include "StandartObject.h"
#include "..\..\materials\Material.h"

class Rectangle : public StandartObject {
public:
    Rectangle(const jsoncons::json& standartObject);
    ~Rectangle();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    void prepareSamples();

private:
    glm::vec3 point;
    glm::vec3 a;			
    glm::vec3 b;
    const Material * material;

    // Precomputed values for fast hit calculation
    glm::vec3 normal;
    float aLenghtSquared;
    float bLenghtSquared;
};

#endif