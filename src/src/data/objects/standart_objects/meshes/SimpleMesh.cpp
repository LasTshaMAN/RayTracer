#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(LMesh& mesh, L3DS& meshLoader) : Mesh(mesh, meshLoader) {
    prepareBoundingBox();
}

SimpleMesh::~SimpleMesh() {
}

bool SimpleMesh::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    if (!boundingBox.hit(ray)) {
        return false;
    }

    bool result = false;

    ObjectIntersectionInfo lastIntersectionInfo;
    float tMin = FLT_MAX;

    for (auto triangle = triangles.begin(); triangle != triangles.end(); ++triangle) {
        if ((*triangle)->hit(ray, lastIntersectionInfo) && lastIntersectionInfo.t < tMin) {
            result = true;
            objectIntersectionInfo = lastIntersectionInfo;
            tMin = lastIntersectionInfo.t;
        }
    }

    return result;
}

bool SimpleMesh::shadowHit(const Ray& ray, float& IntersectionT) const {
    if (!boundingBox.hit(ray)) {
        return false;
    }

    for (auto triangle = triangles.begin(); triangle != triangles.end(); ++triangle) {
        if ((*triangle)->shadowHit(ray, IntersectionT)) {
            return true;
        }
    }

    return false;
}

BoundingBox SimpleMesh::getBoundingBox() const {
    return boundingBox;
}

void SimpleMesh::prepareBoundingBox() {
    float xMin = FLT_MAX, yMin = FLT_MAX, zMin = FLT_MAX;
    float xMax = FLT_MIN, yMax = FLT_MIN, zMax = FLT_MIN;

    for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
        
        if (vertex->x < xMin) {
            xMin = vertex->x;
        } else if (vertex->x > xMax) {
            xMax = vertex->x;
        }

        if (vertex->y < yMin) {
            yMin = vertex->y;
        } else if (vertex->y > yMax) {
            yMax = vertex->y;
        }

        if (vertex->z < zMin) {
            zMin = vertex->z;
        } else if (vertex->z > zMax) {
            zMax = vertex->z;
        }
    }

    boundingBox = BoundingBox(xMin, yMin, zMin, xMax, yMax, zMax);
}