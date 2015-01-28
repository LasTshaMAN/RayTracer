#ifndef __STANDART_OBJECT__
#define __STANDART_OBJECT__

#include "..\..\..\utils\Ray.h"
#include "..\..\..\utils\ObjectIntersectionInfo.h"
#include "..\..\..\utils\BoundingBox.h"

class StandartObject {
public:
    virtual ~StandartObject() {}

    // Ray is considered to be in local coordiante system of an object
    virtual bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const = 0;

    // Ray is considered to be in local coordiante system of an object
    virtual bool shadowHit(const Ray& ray, float& IntersectionT) const = 0;

    const std::vector<glm::vec3>& getSamples() const {
        return samples;
    }

    virtual BoundingBox getBoundingBox() const = 0;
    
protected:
    std::vector<glm::vec3> samples;
};

#endif