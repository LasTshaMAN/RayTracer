#ifndef __SIMPLE_MESH__
#define __SIMPLE_MESH__

#include "l3ds\l3ds.h"

#include "Mesh.h"

class SimpleMesh : public Mesh {
public:
    SimpleMesh(LMesh& mesh, L3DS& meshLoader);
    ~SimpleMesh();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    void prepareBoundingBox();

private:
    BoundingBox boundingBox;
};

#endif