#include "Disk.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#include "..\..\materials\Lambertian.h"
#include "..\..\materials\Emitting.h"
#include "..\..\materials\Phong.h"
#include "..\..\materials\Reflective.h"

using namespace jsoncons;
using namespace std;

Disk::Disk(const json& standartObject) {
    this->radius = static_cast<float>(standartObject["radius"].as<double>());

    const json& material = standartObject["material"];
    string className = material["className"].as<string>();
    if (className == "Lambertian") {
        this->material = new Lambertian(material);
    } else if (className == "Emitting") {
        this->material = new Emitting(material);
    } else if (className == "Phong") {
        this->material = new Phong(material);
    } else if (className == "Reflective") {
        this->material = new Reflective(material);
    }

    this->prepareSamples();
}

Disk::~Disk() {
    delete material;
}

static const float EPSILON = 0.001f;

bool Disk::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    float t = -ray.origin.z / ray.direction.z;

    if (t <= EPSILON) {
        return false;
    }

    glm::vec3 point = ray.origin + t * ray.direction;
    if (glm::dot(point, point) < radius * radius) {
        objectIntersectionInfo.t = t;
        objectIntersectionInfo.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        objectIntersectionInfo.material = material;
        return true;
    }

    return false;
}

bool Disk::shadowHit(const Ray& ray, float& IntersectionT) const {
    float t = -ray.origin.z / ray.direction.z;

    if (t <= EPSILON) {
        return false;
    }

    glm::vec3 point = ray.origin + t * ray.direction;
    if (glm::dot(point, point) < radius * radius) {
        IntersectionT = t;
        return true;
    }

    return false;
}

void Disk::prepareSamples() {
    const float circlesAmount = 5;
    const float amountPerCircle = 10;

    // Add center point
    samples.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    float r = radius / circlesAmount;
    for (int i = 0; i < circlesAmount; ++i) {
        for (int j = 0; j < amountPerCircle; ++j) {
            float phi = j * (2.0f * M_PI / amountPerCircle);
            samples.push_back(glm::vec3(r * cos(phi), r * sin(phi), 0.0f));
        }
        r += radius / circlesAmount;
    }
}

BoundingBox Disk::getBoundingBox() const {
    float delta = 0.0001f;

    return BoundingBox(-radius, -radius, -delta, radius, radius, delta);
}