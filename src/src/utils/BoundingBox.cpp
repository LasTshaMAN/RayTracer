#include "BoundingBox.h"

BoundingBox::BoundingBox() {
    this->x0 = 0;
    this->y0 = 0;
    this->z0 = 0;

    this->x1 = 0;
    this->y1 = 0;
    this->z1 = 0;
}

BoundingBox::BoundingBox(const float x0, const float y0, const float z0,
                         const float x1, const float y1, const float z1) {
    
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;

    this->x1 = x1;
    this->y1 = y1;
    this->z1 = z1;
}

BoundingBox::~BoundingBox() {
}

static const float EPSILON = 0.001f;

bool BoundingBox::hit(const Ray& ray) const {
    float ox = ray.origin.x; float oy = ray.origin.y; float oz = ray.origin.z;
    float dx = ray.direction.x; float dy = ray.direction.y; float dz = ray.direction.z;

    float txMin, tyMin, tzMin;
    float txMax, tyMax, tzMax;

    float a = 1.0f / dx;
    if (a >= 0.0f) {
        txMin = (x0 - ox) * a;
        txMax = (x1 - ox) * a;
    } else {
        txMin = (x1 - ox) * a;
        txMax = (x0 - ox) * a;
    }

    float b = 1.0f / dy;
    if (b >= 0.0f) {
        tyMin = (y0 - oy) * b;
        tyMax = (y1 - oy) * b;
    } else {
        tyMin = (y1 - oy) * b;
        tyMax = (y0 - oy) * b;
    }

    float c = 1.0f / dz;
    if (c >= 0.0f) {
        tzMin = (z0 - oz) * c;
        tzMax = (z1 - oz) * c;
    } else {
        tzMin = (z1 - oz) * c;
        tzMax = (z0 - oz) * c;
    }

    float t0, t1;

    // find largest entering t value

    if (txMin > tyMin) {
        t0 = txMin;
    } else {
        t0 = tyMin;
    }
    
    if (tzMin > t0) {
        t0 = tzMin;
    }

    // find smallest exiting t value

    if (txMax < tyMax) {
        t1 = txMax;
    } else {
        t1 = tyMax;
    }

    if (tzMax < t1) {
        t1 = tzMax;
    }

    return t0 < t1 && t1 > EPSILON;
}

bool BoundingBox::inside(const glm::vec3& point) const {
    return ((point.x > x0 && point.x < x1) && (point.y > y0 && point.y < y1) && (point.z > z0 && point.z < z1));
}