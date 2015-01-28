#ifndef __ACCELERATED_MESH__
#define __ACCELERATED_MESH__

#include "l3ds\l3ds.h"

#include "Mesh.h"
#include "..\..\..\..\utils\Grid.h"

class AcceleratedMesh : public Mesh {
public:
    AcceleratedMesh(LMesh& mesh, L3DS& meshLoader);
    ~AcceleratedMesh();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    Grid<Triangle> grid;
};

#endif