#ifndef __MATERIAL__
#define __MATERIAL__

#include "glm\glm.hpp"
#include "..\..\utils\ObjectIntersectionInfo.h"

class Material {
public:
    virtual ~Material() {}

    virtual glm::vec3 calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const = 0;
};

#endif