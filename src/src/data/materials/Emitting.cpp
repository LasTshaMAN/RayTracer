#include "Emitting.h"

#include "..\DataManager.h"
#include "..\..\tracer\Tracer.h"

Emitting::Emitting(const jsoncons::json& material) {
    const jsoncons::json& color = material["color"];
    this->color = glm::vec3(static_cast<float>(color["red"].as<double>()),
                            static_cast<float>(color["green"].as<double>()),
                            static_cast<float>(color["blue"].as<double>()));
}

Emitting::~Emitting() {
}

glm::vec3 Emitting::calculateColor(const ObjectIntersectionInfo& objectIntersectionInfo, const int rayDepth) const {
    glm::vec3 ambientComponent = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 resultingColor = ambientComponent + color;

    return resultingColor;
}