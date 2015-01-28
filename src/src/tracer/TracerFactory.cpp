#include "TracerFactory.h"

#include <string>

#include "jsoncons\json.hpp"

#include "SimpleTracer.h"
#include "AcceleratedTracer.h"

using namespace std;
using namespace jsoncons;

const TracerFactory TracerFactory::instance = TracerFactory();

TracerFactory::TracerFactory() {
    string configFileName = "resources/configurations/TracerConfiguration.json";

    json configFile = json::parse_file(configFileName);

    try {
        bool enableGridAcceleration = configFile["enableGridAcceleration"].as<bool>();
        if (enableGridAcceleration) {
            tracer = new AcceleratedTracer(configFile);
        } else {
            tracer = new SimpleTracer(configFile);
        }

    }
    catch (const json_exception& e) {
        cerr << "Unable to read " + configFileName << endl;
        cerr << e.what() << endl;
    }
}

TracerFactory::~TracerFactory() {
    delete tracer;
}

const Tracer * TracerFactory::getTracer() const {
    return tracer;
}

const TracerFactory& TracerFactory::getTracerFactory() {
    return instance;
}