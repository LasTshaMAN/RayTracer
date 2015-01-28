#include "AcceleratedTracer.h"

#include "..\data\DataManager.h"

AcceleratedTracer::AcceleratedTracer(const jsoncons::json& tracerConfig) : Tracer(tracerConfig),
    grid(DataManager::getDataManager().getObjectInstances()) {
    
    // Do nothing
}

AcceleratedTracer::~AcceleratedTracer() {
}

glm::vec3 AcceleratedTracer::traceRay(const Ray& ray, const int rayDepth) const {
    if (rayDepth > maxRayDepth) {
        return backgroundColor;
    }

    ObjectIntersectionInfo objectIntersectionInfo;
    if (grid.hit(ray, objectIntersectionInfo)) {
        return objectIntersectionInfo.material->calculateColor(objectIntersectionInfo, rayDepth);
    } else {
        return backgroundColor;
    }
}

static const float EPSILON = 0.5f;

bool AcceleratedTracer::shadowRayIntersectsObject(const Ray& ray, const float lightHitPointDistance) const {
    if (!shadowsEnabled) {
        return false;
    }

    float IntersectionT = FLT_MAX;
    if (grid.shadowHit(ray, IntersectionT) && (IntersectionT + EPSILON < lightHitPointDistance)) {
        return true;
    }
    
    return false;
}