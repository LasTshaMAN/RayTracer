#ifndef __OBJECT_INSTANCE__
#define __OBJECT_INSTANCE__

#include "jsoncons\json.hpp"

#include "..\..\utils\Ray.h"
#include "..\..\utils\ObjectIntersectionInfo.h"
#include "standart_objects\StandartObject.h"
#include "..\materials\Material.h"

class ObjectInstance {
public:
    ObjectInstance(const jsoncons::json& objectInstance, const StandartObject * standartObject);
    ~ObjectInstance();

    // Ray is considered to be in global coordiante system of the world
    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    // Ray is considered to be in global coordiante system of the world
    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    const std::vector<glm::vec3>& getSamples() const;
    BoundingBox getBoundingBox() const;

private:
    void scale(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void translate(float x, float y, float z);
    void prepareSamples();
    void prepareBoundingBox();

private:
    const StandartObject * standartObject;
    glm::mat4 forwardMatrix;
    glm::mat4 inverseMatrix;
    std::vector<glm::vec3> samples;
    BoundingBox boundingBox;
};

#endif