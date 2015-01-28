#include "Rectangle.h"

#include <algorithm>

#include "..\..\materials\Lambertian.h"
#include "..\..\materials\Emitting.h"
#include "..\..\materials\Phong.h"
#include "..\..\materials\Reflective.h"

using namespace std;
using namespace jsoncons;

Rectangle::Rectangle(const jsoncons::json& standartObject) {
    const jsoncons::json& point = standartObject["point"];
    this->point = glm::vec3(static_cast<float>(point["x"].as<double>()),
                            static_cast<float>(point["y"].as<double>()),
                            static_cast<float>(point["z"].as<double>()));

    const jsoncons::json& a = standartObject["a"];
    this->a = glm::vec3(static_cast<float>(a["x"].as<double>()),
                        static_cast<float>(a["y"].as<double>()),
                        static_cast<float>(a["z"].as<double>()));

    const jsoncons::json& b = standartObject["b"];
    this->b = glm::vec3(static_cast<float>(b["x"].as<double>()),
                        static_cast<float>(b["y"].as<double>()),
                        static_cast<float>(b["z"].as<double>()));

    this->normal = glm::normalize(glm::cross(this->a, this->b));
    this->aLenghtSquared = glm::dot(this->a, this->a);
    this->bLenghtSquared = glm::dot(this->b, this->b);

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

Rectangle::~Rectangle() {
    delete material;
}

static const float EPSILON = 0.001f;

bool Rectangle::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    float t = glm::dot((point - ray.origin), normal) / glm::dot(ray.direction, normal);

    if (t <= EPSILON) {
        return false;
    }
    glm::vec3 p = ray.origin + t * ray.direction;
    glm::vec3 d = p - point;

    float dDotA = glm::dot(d, a);
    if (dDotA < 0.0f || dDotA > aLenghtSquared) {
        return false;
    }
    float dDotB = glm::dot(d, b);
    if (dDotB < 0.0f || dDotB > bLenghtSquared) {
        return false;
    }

    objectIntersectionInfo.t = t;
    objectIntersectionInfo.normal = normal;
    objectIntersectionInfo.material = material;

    return true;
}

bool Rectangle::shadowHit(const Ray& ray, float& IntersectionT) const {
    float t = glm::dot((point - ray.origin), normal) / glm::dot(ray.direction, normal);

    if (t <= EPSILON) {
        return false;
    }
    glm::vec3 p = ray.origin + t * ray.direction;
    glm::vec3 d = p - point;

    float dDotA = glm::dot(d, a);
    if (dDotA < 0.0f || dDotA > aLenghtSquared) {
        return false;
    }
    float dDotB = glm::dot(d, b);
    if (dDotB < 0.0f || dDotB > bLenghtSquared) {
        return false;
    }

    IntersectionT = t;

    return true;
}

void Rectangle::prepareSamples() {
    const int amountPerRow = 5;

    const float aLenght = sqrt(aLenghtSquared);
    const float bLenght = sqrt(bLenghtSquared);
    for (int i = 0; i < amountPerRow; ++i) {
        for (int j = 0; j < amountPerRow; ++j) {
            glm::vec3 currentSample = point + ((i + 0.5f) * (aLenght / amountPerRow)) * a +
                ((j + 0.5f) * (bLenght / amountPerRow)) * b;
            samples.push_back(currentSample);
        }
    }
}

BoundingBox Rectangle::getBoundingBox() const {
    float delta = 0.0001f; 

    return BoundingBox(min(point.x, point.x + a.x + b.x) - delta, min(point.y, point.y + a.y + b.y) - delta,
                       min(point.z, point.z + a.z + b.z) - delta, max(point.x, point.x + a.x + b.x) + delta,
				       max(point.y, point.y + a.y + b.y) + delta, max(point.z, point.z + a.z + b.z) + delta);
}