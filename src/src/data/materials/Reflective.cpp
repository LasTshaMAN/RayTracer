#include "Reflective.h"

#include "..\DataManager.h"
#include "..\..\tracer\TracerFactory.h"

Reflective::Reflective(const jsoncons::json& material) {
    const jsoncons::json& reflectionCoefficient = material["reflectionCoefficient"];
    this->reflectionCoefficient = glm::vec3(static_cast<float>(reflectionCoefficient["red"].as<double>()),
                                            static_cast<float>(reflectionCoefficient["green"].as<double>()),
                                            static_cast<float>(reflectionCoefficient["blue"].as<double>()));
}

Reflective::Reflective(glm::vec3 reflectionCoefficient) {
    this->reflectionCoefficient = reflectionCoefficient;
}

Reflective::~Reflective() {
}

static const float EPSILON = 0.5f;

glm::vec3 Reflective::calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const {
    glm::vec3 reflectedDir = glm::normalize(objectIntersectionInfo.ray.direction + 2.0f *
        glm::dot(objectIntersectionInfo.normal, -objectIntersectionInfo.ray.direction) * objectIntersectionInfo.normal);
    glm::vec3 hitPoint = objectIntersectionInfo.ray.origin + objectIntersectionInfo.t * objectIntersectionInfo.ray.direction;
    Ray reflectedRay(hitPoint + EPSILON * reflectedDir, reflectedDir);
    return reflectionCoefficient * TracerFactory::getTracerFactory().getTracer()->traceRay(reflectedRay, rayDepth + 1);
}