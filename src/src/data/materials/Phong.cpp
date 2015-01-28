#include "Phong.h"

#include "..\DataManager.h"
#include "..\..\tracer\TracerFactory.h"

Phong::Phong(const jsoncons::json& material) {
    const jsoncons::json& diffuseColor = material["diffuseColor"];
    this->diffuseColor = glm::vec3(static_cast<float>(diffuseColor["red"].as<double>()),
                                   static_cast<float>(diffuseColor["green"].as<double>()),
                                   static_cast<float>(diffuseColor["blue"].as<double>()));

    const jsoncons::json& specularColor = material["specularColor"];
    this->specularColor = glm::vec3(static_cast<float>(specularColor["red"].as<double>()),
                                   static_cast<float>(specularColor["green"].as<double>()),
                                   static_cast<float>(specularColor["blue"].as<double>()));

    this->shininess = static_cast<float>(material["shininess"].as<double>());
}

Phong::Phong(glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess) {
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->shininess = shininess;
}

Phong::~Phong() {
}

static const float EPSILON = 0.5f;

glm::vec3 Phong::calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const {
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
                    
                    glm::vec3 reflectedDir = (-lightDirection + 2.0f * normalDotLightDirection * objectIntersectionInfo.normal);
                    float reflectedDirDotCameraDir = glm::dot(reflectedDir, -objectIntersectionInfo.ray.direction);
                    if (reflectedDirDotCameraDir < 0) {
                        reflectedDirDotCameraDir = 0;
                    }

                    if (lightIntensity < lightHitPointDistance * lightHitPointDistance) {
                        totalLightSourceColor += (*lightSource)->getColor() * (diffuseColor * normalDotLightDirection +
                            specularColor * pow(reflectedDirDotCameraDir, shininess)) *
                            (lightIntensity / (lightHitPointDistance *lightHitPointDistance));
                    } else {
                        totalLightSourceColor += (*lightSource)->getColor() * (diffuseColor * normalDotLightDirection +
                            specularColor * pow(reflectedDirDotCameraDir, shininess));
                    }
                }
            }
        }
        resultingColor += (totalLightSourceColor / static_cast<float>(samples.size()));
    }

    return resultingColor;
}