#include "Sphere.h"

#include "..\..\materials\Lambertian.h"
#include "..\..\materials\Emitting.h"
#include "..\..\materials\Phong.h"
#include "..\..\materials\Reflective.h"

using namespace jsoncons;
using namespace std;

Sphere::Sphere(const json& standartObject) {
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
}

Sphere::~Sphere() {
}

static const float EPSILON = 0.001f;

bool Sphere::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    glm::vec3 temp = ray.origin;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(temp, ray.direction);
    float c = glm::dot(temp, temp) - radius * radius;
    float disc = static_cast<float>(b * b - 4.0 * a * c);

    if (disc >= 0.0f) {
        float t;
        float e = sqrt(disc);
        float denom = 2.0f * a;

        t = (-b - e) / denom;    // Smaller root

        if (t > EPSILON) {
            objectIntersectionInfo.t = t;
            objectIntersectionInfo.normal = (temp + t * ray.direction) / radius;
            objectIntersectionInfo.material = material;
            return true;
        }

        t = (-b + e) / denom;    // Larger root

        if (t > EPSILON) {
            objectIntersectionInfo.t = t;
            objectIntersectionInfo.normal = (temp + t * ray.direction) / radius;
            objectIntersectionInfo.material = material;
            return true;
        }
    }

    return false;
}

bool Sphere::shadowHit(const Ray& ray, float& IntersectionT) const {
    glm::vec3 temp = ray.origin;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(temp, ray.direction);
    float c = glm::dot(temp, temp) - radius * radius;
    float disc = static_cast<float>(b * b - 4.0 * a * c);

    if (disc >= 0.0f) {
        float t;
        float e = sqrt(disc);
        float denom = 2.0f * a;

        t = (-b - e) / denom;    // Smaller root

        if (t > EPSILON) {
            IntersectionT = t;
            return true;
        }

        t = (-b + e) / denom;    // Larger root

        if (t > EPSILON) {
            IntersectionT = t;
            return true;
        }
    }

    return false;
}

BoundingBox Sphere::getBoundingBox() const {
    return BoundingBox(-radius, -radius, -radius,
        radius, radius, radius);
}