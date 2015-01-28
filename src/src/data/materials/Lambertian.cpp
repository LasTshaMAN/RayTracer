#include "Lambertian.h"

#include "..\DataManager.h"
#include "..\..\tracer\TracerFactory.h"

Lambertian::Lambertian(const jsoncons::json& material) {
    const jsoncons::json& color = material["color"];
    this->color = glm::vec3(static_cast<float>(color["red"].as<double>()),
                            static_cast<float>(color["green"].as<double>()),
                            static_cast<float>(color["blue"].as<double>()));
}

Lambertian::Lambertian(glm::vec3 color) {
    this->color = color;
}

Lambertian::~Lambertian() {
}

static const float EPSILON = 0.5f;

glm::vec3 Lambertian::calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const {
    glm::vec3 ambientComponent = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 resultingColor = ambientComponent;

    glm::vec3 hitPoint = objectIntersectionInfo.ray.origin + objectIntersectionInfo.t * objectIntersectionInfo.ray.direction;
    const std::vector<LightSource *>& lightSources = DataManager::getDataManager().getLightSources();
    for (auto lightSource = lightSources.begin(); lightSource != lightSources.end(); ++lightSource) {
        const std::vector<glm::vec3>& samples = (*lightSource)->getSamples();
        float lightIntensity = (*lightSource)->getIntensity();

        glm::vec3 totalLightSourceColor(0.0f, 0.0f, 0.0f);
        for (auto sample = samples.begin(); sample != samples.end(); ++sample) {
            glm::vec3 lightDirection = glm::normalize((*sample) - hitPoint);
            float normalDotLightDirection = glm::dot(objectIntersectionInfo.normal, lightDirection);

            if (normalDotLightDirection > 0.0f) {
                Ray shadowRay(hitPoint + EPSILON * lightDirection, lightDirection);
                float lightHitPointDistance = glm::distance(*sample, hitPoint + EPSILON * lightDirection);
                if (!TracerFactory::getTracerFactory().getTracer()->shadowRayIntersectsObject(shadowRay, lightHitPointDistance)) {
                    if (lightIntensity < lightHitPointDistance * lightHitPointDistance) {
                        totalLightSourceColor += (*lightSource)->getColor() * color * normalDotLightDirection *
                            (lightIntensity / (lightHitPointDistance *lightHitPointDistance));
                    } else {
                        totalLightSourceColor += (*lightSource)->getColor() * color * normalDotLightDirection;
                    }
                }
            }
        }
        resultingColor += (totalLightSourceColor / static_cast<float>(samples.size()));
    }

    return resultingColor;
}