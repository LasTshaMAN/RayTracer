#ifndef __EMITTING__
#define __EMITTING__

#include "jsoncons\json.hpp"

#include "Material.h"

class Emitting : public Material {
public:
    Emitting(const jsoncons::json& material);
    ~Emitting();

    glm::vec3 calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const;

private:
    glm::vec3 color;
};

#endif