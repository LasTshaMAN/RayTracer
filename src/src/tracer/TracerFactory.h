#ifndef __TRACER_FACTORY__
#define __TRACER_FACTORY__

#include "glm\glm.hpp"

#include "Tracer.h"

class TracerFactory {
public:
    const Tracer * getTracer() const;

private:
    Tracer * tracer;

public:
    static const TracerFactory& getTracerFactory();
private:
    TracerFactory();
    ~TracerFactory();

    static const TracerFactory instance;
};

#endif