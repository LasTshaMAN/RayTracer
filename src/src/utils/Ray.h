#ifndef __RAY__
#define __RAY__

#include "glm\glm.hpp"

class Ray {
public:
    Ray();
    Ray(glm::vec3 origin, glm::vec3 direction);
    ~Ray();

public:
    glm::vec3 origin;
    glm::vec3 direction;
};

#endif