#include "AcceleratedMesh.h"

AcceleratedMesh::AcceleratedMesh(LMesh& mesh, L3DS& meshLoader) : Mesh(mesh, meshLoader), grid(triangles) {
    // Do nothing
}

AcceleratedMesh::~AcceleratedMesh() {
}

bool AcceleratedMesh::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    if (grid.hit(ray, objectIntersectionInfo)) {
        return true;
    } else {
        return false;
    }
}

bool AcceleratedMesh::shadowHit(const Ray& ray, float& IntersectionT) const {
    if (grid.shadowHit(ray, IntersectionT)) {
        return true;
    }
    else {
        return false;
    }
}

BoundingBox AcceleratedMesh::getBoundingBox() const {
    return grid.getBoundingBox();
}