#ifndef __OBJECT_INTERSECTION_INFO__
#define __OBJECT_INTERSECTION_INFO__

#include "glm\glm.hpp"
#include "Ray.h"

class Material;

class ObjectIntersectionInfo {
public:
    ObjectIntersectionInfo();
    ~ObjectIntersectionInfo();

public:
    glm::vec3 normal;
    const Material * material;
    float t;
    Ray ray;
};

#endif