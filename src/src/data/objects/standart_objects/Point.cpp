#include "Point.h"

Point::Point(const jsoncons::json& standartObject) {
    const jsoncons::json& point = standartObject["point"];
    this->point = glm::vec3(static_cast<float>(point["x"].as<double>()),
                            static_cast<float>(point["y"].as<double>()),
                            static_cast<float>(point["z"].as<double>()));

    this->prepareSamples();
}

Point::~Point() {
}

bool Point::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    // We don't want to display a point
    return false;
}

bool Point::shadowHit(const Ray& ray, float& IntersectionT) const {
    // We don't want to take into consideration a point
    return false;
}

BoundingBox Point::getBoundingBox() const {
    float delta = 0.0001f;
    
    return BoundingBox(point.x - delta, point.y - delta, point.z - delta,
                       point.x + delta, point.y + delta, point.z + delta);
}

void Point::prepareSamples() {
    samples.push_back(point);
}