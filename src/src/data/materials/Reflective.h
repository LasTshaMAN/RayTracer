#ifndef __REFLECTIVE__
#define __REFLECTIVE__

#include "jsoncons\json.hpp"

#include "Material.h"

class Reflective : public Material {
public:
    Reflective(const jsoncons::json& material);
    Reflective(glm::vec3 reflectionCoefficient);
    ~Reflective();

    glm::vec3 calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const;

private:
    glm::vec3 reflectionCoefficient;
};

#endif