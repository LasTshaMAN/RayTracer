#ifndef __LIGHT_SOURCE__
#define __LIGHT_SOURCE__

#include "jsoncons\json.hpp"

#include "..\objects\ObjectInstance.h"

class LightSource {
public:
    LightSource(const jsoncons::json& lightSource, const ObjectInstance * geometricShape);
    ~LightSource();

    const glm::vec3& getColor() const;
    const float getIntensity() const;

    const std::vector<glm::vec3>& getSamples() const;

private:
    const ObjectInstance * geometricShape;
    glm::vec3 color;
    float intensity;
};

#endif