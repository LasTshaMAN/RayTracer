#ifndef __PHONG__
#define __PHONG__

#include "jsoncons\json.hpp"

#include "Material.h"

class Phong : public Material {
public:
    Phong(const jsoncons::json& material);
    Phong(glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess);
    ~Phong();

    glm::vec3 calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const;

private:
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};

#endif