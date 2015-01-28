#ifndef __SIMPLE_TRACER__
#define __SIMPLE_TRACER__

#include "glm\glm.hpp"

#include "Tracer.h"
#include "..\utils\Ray.h"
#include "..\utils\ObjectIntersectionInfo.h"

class SimpleTracer : public Tracer {
public:
    SimpleTracer(const jsoncons::json& tracerConfig);
    ~SimpleTracer();

    glm::vec3 traceRay(const Ray& ray, const int rayDepth) const;
    bool shadowRayIntersectsObject(const Ray& ray, const float lightHitPointDistance) const;

private:
    bool intersectsObject(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;
};

#endif