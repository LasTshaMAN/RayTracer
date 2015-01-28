#include "Tracer.h"

#include <string>

#include "jsoncons\json.hpp"

using namespace std;
using namespace jsoncons;

Tracer::Tracer(const jsoncons::json& tracerConfig) {
    this->maxRayDepth = tracerConfig["maxRayDepth"].as<int>();

    this->shadowsEnabled = tracerConfig["shadowsEnabled"].as<bool>();

    json backgroundColor = tracerConfig["backgroundColor"];
    float red = static_cast<float>(backgroundColor["red"].as<double>());
    float green = static_cast<float>(backgroundColor["green"].as<double>());
    float blue = static_cast<float>(backgroundColor["blue"].as<double>());
    this->backgroundColor = glm::vec3(red, green, blue);
}

Tracer::~Tracer() {
}