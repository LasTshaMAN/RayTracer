#ifndef __TRACER__
#define __TRACER__

#include "glm\glm.hpp"
#include "jsoncons\json.hpp"

#include "..\utils\Ray.h"
#include "..\utils\ObjectIntersectionInfo.h"

class Tracer {
public:
    Tracer(const jsoncons::json& tracerConfig);
    virtual ~Tracer();

    virtual glm::vec3 traceRay(const Ray& ray, const int rayDepth) const = 0;
    virtual bool shadowRayIntersectsObject(const Ray& ray, const float lightHitPointDistance) const = 0;

protected:
    int maxRayDepth;
    bool shadowsEnabled;
    glm::vec3 backgroundColor;
};

#endif