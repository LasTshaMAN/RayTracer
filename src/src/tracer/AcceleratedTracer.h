#ifndef __GRID_TRACER__
#define __GRID_TRACER__

#include "glm\glm.hpp"

#include "Tracer.h"
#include "..\data\objects\ObjectInstance.h"
#include "..\utils\Ray.h"
#include "..\utils\ObjectIntersectionInfo.h"
#include "..\utils\Grid.h"

class AcceleratedTracer : public Tracer {
public:
    AcceleratedTracer(const jsoncons::json& tracerConfig);
    ~AcceleratedTracer();

    glm::vec3 traceRay(const Ray& ray, const int rayDepth) const;
    bool shadowRayIntersectsObject(const Ray& ray, const float lightHitPointDistance) const;

private:
    Grid<ObjectInstance> grid;
};

#endif