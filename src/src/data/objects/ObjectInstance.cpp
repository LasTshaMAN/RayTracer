#include "ObjectInstance.h"

#include <glm\gtc\matrix_transform.hpp>

ObjectInstance::ObjectInstance(const jsoncons::json& objectInstance,
    const StandartObject * standartObject) {

    this->standartObject = standartObject;
    if (objectInstance.has_member("transformations")) {
        const jsoncons::json& transformations = objectInstance["transformations"];
        if (transformations.has_member("scale")) {
            const jsoncons::json& scale = transformations["scale"];
            float x = static_cast<float>(scale["x"].as<double>());
            float y = static_cast<float>(scale["y"].as<double>());
            float z = static_cast<float>(scale["z"].as<double>());
            this->scale(x, y, z);
        }
        if (transformations.has_member("rotate")) {
            const jsoncons::json& rotate = transformations["rotate"];
            for (int j = 0; j < rotate.size(); ++j) {
                const jsoncons::json& currentRotation = rotate[j];

                float angle = static_cast<float>(currentRotation["angle"].as<double>());
                float x = static_cast<float>(currentRotation["x"].as<double>());
                float y = static_cast<float>(currentRotation["y"].as<double>());
                float z = static_cast<float>(currentRotation["z"].as<double>());
                this->rotate(angle, x, y, z);
            }
        }
        if (transformations.has_member("translate")) {
            const jsoncons::json& translate = transformations["translate"];
            float x = static_cast<float>(translate["x"].as<double>());
            float y = static_cast<float>(translate["y"].as<double>());
            float z = static_cast<float>(translate["z"].as<double>());
            this->translate(x, y, z);
        }
    }

    this->prepareSamples();
    this->prepareBoundingBox();
}

ObjectInstance::~ObjectInstance() {
}

bool ObjectInstance::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    Ray inv_ray(glm::vec3(inverseMatrix * glm::vec4(ray.origin, 1.0f)), glm::vec3(inverseMatrix * glm::vec4(ray.direction, 0.0f)));

    if (standartObject->hit(inv_ray, objectIntersectionInfo)) {
        objectIntersectionInfo.normal = glm::normalize(glm::vec3(glm::transpose(inverseMatrix) *
            glm::vec4(objectIntersectionInfo.normal, 0.0f)));
        objectIntersectionInfo.ray = ray;
        return true;
    }

    return false;
}

bool ObjectInstance::shadowHit(const Ray& ray, float& IntersectionT) const {
    Ray inv_ray(glm::vec3(inverseMatrix * glm::vec4(ray.origin, 1.0f)), glm::vec3(inverseMatrix * glm::vec4(ray.direction, 0.0f)));

    if (standartObject->shadowHit(inv_ray, IntersectionT)) {
        return true;
    }

    return false;
}

const std::vector<glm::vec3>& ObjectInstance::getSamples() const {
    return samples;
}

BoundingBox ObjectInstance::getBoundingBox() const {
    return boundingBox;
}

void ObjectInstance::scale(float x, float y, float z) {
    glm::mat4 forwardScalingMatrix;
    forwardMatrix = glm::scale(forwardScalingMatrix, glm::vec3(x, y, z)) * forwardMatrix;
    glm::mat4 invScalingMatrix;
    inverseMatrix = inverseMatrix * glm::inverse(glm::scale(invScalingMatrix, glm::vec3(x, y, z)));
}

void ObjectInstance::rotate(float angle, float x, float y, float z) {
    glm::mat4 forwardRotationMatrix;
    forwardMatrix = glm::rotate(forwardRotationMatrix, angle, glm::vec3(x, y, z)) * forwardMatrix;
    glm::mat4 invRotationMatrix;
    inverseMatrix = inverseMatrix * glm::inverse(glm::rotate(invRotationMatrix, angle, glm::vec3(x, y, z)));
}

void ObjectInstance::translate(float x, float y, float z) {
    glm::mat4 forwardTranslationMatrix;
    forwardMatrix = glm::translate(forwardTranslationMatrix, glm::vec3(x, y, z)) * forwardMatrix;
    glm::mat4 invTranslationMatrix;
    inverseMatrix = inverseMatrix * glm::inverse(glm::translate(invTranslationMatrix, glm::vec3(x, y, z)));
}

void ObjectInstance::prepareSamples() {
    const std::vector<glm::vec3>& standartSamples = standartObject->getSamples();
    samples.reserve(standartSamples.size());
    for (auto sample = standartSamples.begin(); sample != standartSamples.end(); ++sample) {
        samples.push_back(glm::vec3(forwardMatrix * glm::vec4(*sample, 1.0f)));
    }
}

void ObjectInstance::prepareBoundingBox() {
    BoundingBox objectBBox = standartObject->getBoundingBox();

    glm::vec3 v[8];

    v[0].x = objectBBox.x0; v[0].y = objectBBox.y0; v[0].z = objectBBox.z0;
    v[1].x = objectBBox.x1; v[1].y = objectBBox.y0; v[1].z = objectBBox.z0;
    v[2].x = objectBBox.x1; v[2].y = objectBBox.y1; v[2].z = objectBBox.z0;
    v[3].x = objectBBox.x0; v[3].y = objectBBox.y1; v[3].z = objectBBox.z0;

    v[4].x = objectBBox.x0; v[4].y = objectBBox.y0; v[4].z = objectBBox.z1;
    v[5].x = objectBBox.x1; v[5].y = objectBBox.y0; v[5].z = objectBBox.z1;
    v[6].x = objectBBox.x1; v[6].y = objectBBox.y1; v[6].z = objectBBox.z1;
    v[7].x = objectBBox.x0; v[7].y = objectBBox.y1; v[7].z = objectBBox.z1;

    v[0] = glm::vec3(forwardMatrix * glm::vec4(v[0], 1.0f));
    v[1] = glm::vec3(forwardMatrix * glm::vec4(v[1], 1.0f));
    v[2] = glm::vec3(forwardMatrix * glm::vec4(v[2], 1.0f));
    v[3] = glm::vec3(forwardMatrix * glm::vec4(v[3], 1.0f));
    v[4] = glm::vec3(forwardMatrix * glm::vec4(v[4], 1.0f));
    v[5] = glm::vec3(forwardMatrix * glm::vec4(v[5], 1.0f));
    v[6] = glm::vec3(forwardMatrix * glm::vec4(v[6], 1.0f));
    v[7] = glm::vec3(forwardMatrix * glm::vec4(v[7], 1.0f));

    float x0 = FLT_MAX;
    float y0 = FLT_MAX;
    float z0 = FLT_MAX;

    for (int j = 0; j <= 7; j++)  {
        if (v[j].x < x0) {
            x0 = v[j].x;
        }
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].y < y0) {
            y0 = v[j].y;
        }
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].z < z0) {
            z0 = v[j].z;
        }
    }

    float x1 = FLT_MIN;
    float y1 = FLT_MIN;
    float z1 = FLT_MIN;

    for (int j = 0; j <= 7; j++) {
        if (v[j].x > x1) {
            x1 = v[j].x;
        }
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].y > y1) {
            y1 = v[j].y;
        }
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].z > z1) {
            z1 = v[j].z;
        }
    }

    boundingBox = BoundingBox(x0, y0, z0, x1, y1, z1);
}