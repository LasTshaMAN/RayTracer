#ifndef __BOUNDING_BOX__
#define __BOUNDING_BOX__

#include "glm\glm.hpp"

#include "Ray.h"

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const float x0, const float y0, const float z0,
                const float x1, const float y1, const float z1);
    ~BoundingBox();

    bool hit(const Ray& ray) const;

    bool inside(const glm::vec3& point) const;

public:
    float x0, y0, z0;
    float x1, y1, z1;
};

#endif