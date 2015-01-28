#include "Ray.h"

Ray::Ray() :
    origin(0.0f, 0.0f, 0.0f),
    direction(0.0f, 0.0f, 0.0f)
{}

Ray::Ray(glm::vec3 o, glm::vec3 d) :
    origin(o),
    direction(d)
{}

Ray::~Ray() {}