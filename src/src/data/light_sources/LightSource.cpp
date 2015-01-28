#include "LightSource.h"

LightSource::LightSource(const jsoncons::json& lightSource, const ObjectInstance * geometricShape) {
    this->geometricShape = geometricShape;

    this->intensity = static_cast<float>(lightSource["intensity"].as<double>());

    const jsoncons::json& color = lightSource["color"];
    this->color = glm::vec3(static_cast<float>(color["red"].as<double>()),
                            static_cast<float>(color["green"].as<double>()),
                            static_cast<float>(color["blue"].as<double>()));
}

LightSource::~LightSource() {
}

const glm::vec3& LightSource::getColor() const {
    return color;
}

const float LightSource::getIntensity() const {
    return intensity;
}

const std::vector<glm::vec3>& LightSource::getSamples() const {
    return geometricShape->getSamples();
}