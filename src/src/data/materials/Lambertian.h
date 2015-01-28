#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "jsoncons\json.hpp"

#include "Material.h"

class Lambertian : public Material {
public:
    Lambertian(const jsoncons::json& material);
    Lambertian(glm::vec3 color);
    ~Lambertian();

    glm::vec3 calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const;

private:
    glm::vec3 color;
};

#endif