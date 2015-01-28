#include "SimpleTracer.h"

#include "..\data\DataManager.h"
#include "..\data\objects\ObjectInstance.h"

SimpleTracer::SimpleTracer(const jsoncons::json& tracerConfig) : Tracer(tracerConfig) {
    // Do nothing
}

SimpleTracer::~SimpleTracer() {
}

glm::vec3 SimpleTracer::traceRay(const Ray& ray, const int rayDepth) const {
    if (rayDepth > maxRayDepth) {
        return backgroundColor;
    }

    ObjectIntersectionInfo objectIntersectionInfo;
    if (intersectsObject(ray, objectIntersectionInfo)) {
        return objectIntersectionInfo.material->calculateColor(objectIntersectionInfo, rayDepth);
    }
    else {
        return backgroundColor;
    }
}

static const float EPSILON = 0.5f;

bool SimpleTracer::shadowRayIntersectsObject(const Ray& ray, const float lightHitPointDistance) const {
    if (!shadowsEnabled) {
        return false;
    }
    float IntersectionT = FLT_MAX;

    const std::vector<ObjectInstance *>& objects = DataManager::getDataManager().getObjectInstances();
    for (auto object = objects.begin(); object != objects.end(); ++object) {
        if ((*object)->shadowHit(ray, IntersectionT) && (IntersectionT + EPSILON < lightHitPointDistance)) {
            return true;
        }
    }

    return false;
}

bool SimpleTracer::intersectsObject(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    bool result = false;
    ObjectIntersectionInfo lastIntersectionInfo;
    float tMin = FLT_MAX;

    const std::vector<ObjectInstance *>& objects = DataManager::getDataManager().getObjectInstances();
    for (auto object = objects.begin(); object != objects.end(); ++object) {
        if ((*object)->hit(ray, lastIntersectionInfo) && lastIntersectionInfo.t < tMin) {
            result = true;
            objectIntersectionInfo = lastIntersectionInfo;
            tMin = lastIntersectionInfo.t;
        }
    }

    return result;
}