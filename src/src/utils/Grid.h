#ifndef __GRID__
#define __GRID__

#include <vector>

#include "glm\glm.hpp"

#include "Ray.h"
#include "BoundingBox.h"
#include "ObjectIntersectionInfo.h"

template <class Object>
class Grid {
public:
    Grid(const std::vector<Object *> objects) {
        prepareGrid(objects);
    }
    ~Grid() {
    }

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
        float ox = ray.origin.x;
        float oy = ray.origin.y;
        float oz = ray.origin.z;
        float dx = ray.direction.x;
        float dy = ray.direction.y;
        float dz = ray.direction.z;

        float x0 = boundingBox.x0;
        float y0 = boundingBox.y0;
        float z0 = boundingBox.z0;
        float x1 = boundingBox.x1;
        float y1 = boundingBox.y1;
        float z1 = boundingBox.z1;

        float txMin, tyMin, tzMin;
        float txMax, tyMax, tzMax;

        float a = 1.0f / dx;
        if (a >= 0.0f) {
            txMin = (x0 - ox) * a;
            txMax = (x1 - ox) * a;
        }
        else {
            txMin = (x1 - ox) * a;
            txMax = (x0 - ox) * a;
        }

        float b = 1.0f / dy;
        if (b >= 0.0f) {
            tyMin = (y0 - oy) * b;
            tyMax = (y1 - oy) * b;
        }
        else {
            tyMin = (y1 - oy) * b;
            tyMax = (y0 - oy) * b;
        }

        float c = 1.0f / dz;
        if (c >= 0.0f) {
            tzMin = (z0 - oz) * c;
            tzMax = (z1 - oz) * c;
        }
        else {
            tzMin = (z1 - oz) * c;
            tzMax = (z0 - oz) * c;
        }

        float t0, t1;

        if (txMin > tyMin) {
            t0 = txMin;
        }
        else {
            t0 = tyMin;
        }

        if (tzMin > t0) {
            t0 = tzMin;
        }

        if (txMax < tyMax) {
            t1 = txMax;
        }
        else {
            t1 = tyMax;
        }

        if (tzMax < t1) {
            t1 = tzMax;
        }

        if (t0 > t1) {
            return false;
        }

        // Initial cell coordinates

        int ix, iy, iz;

        if (boundingBox.inside(ray.origin)) {  			// Does the ray start inside the grid?
            ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
            iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
            iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
        }
        else {
            glm::vec3 p = ray.origin + t0 * ray.direction;         // Initial hit point with grid's bounding box
            ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
            iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
            iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
        }

        // Ray parameter increments per cell in the x, y, and z directions

        float dtx = (txMax - txMin) / nx;
        float dty = (tyMax - tyMin) / ny;
        float dtz = (tzMax - tzMin) / nz;

        float 	txNext, tyNext, tzNext;
        int 	ixStep, iyStep, izStep;
        int 	ixStop, iyStop, izStop;

        if (dx > 0.0f) {
            txNext = txMin + (ix + 1) * dtx;
            ixStep = +1;
            ixStop = nx;
        }
        else {
            txNext = txMin + (nx - ix) * dtx;
            ixStep = -1;
            ixStop = -1;
        }

        if (dx == 0.0f) {
            txNext = FLT_MAX;
            ixStep = -1;
            ixStop = -1;
        }


        if (dy > 0.0f) {
            tyNext = tyMin + (iy + 1) * dty;
            iyStep = +1;
            iyStop = ny;
        }
        else {
            tyNext = tyMin + (ny - iy) * dty;
            iyStep = -1;
            iyStop = -1;
        }

        if (dy == 0.0) {
            tyNext = FLT_MAX;
            iyStep = -1;
            iyStop = -1;
        }

        if (dz > 0) {
            tzNext = tzMin + (iz + 1) * dtz;
            izStep = +1;
            izStop = nz;
        }
        else {
            tzNext = tzMin + (nz - iz) * dtz;
            izStep = -1;
            izStop = -1;
        }

        if (dz == 0.0) {
            tzNext = FLT_MAX;
            izStep = -1;
            izStop = -1;
        }

        // Traverse the grid

        while (true) {
            const std::vector<Object *>& cell = cells[ix + nx * iy + nx * ny * iz];

            if (txNext < tyNext && txNext < tzNext) {
                if (hitInsideCell(ray, objectIntersectionInfo, cell) && objectIntersectionInfo.t < txNext) {
                    return true;
                }

                txNext += dtx;
                ix += ixStep;

                if (ix == ixStop) {
                    return false;
                }
            }
            else {
                if (tyNext < tzNext) {
                    if (hitInsideCell(ray, objectIntersectionInfo, cell) && objectIntersectionInfo.t < tyNext) {
                        return true;
                    }

                    tyNext += dty;
                    iy += iyStep;

                    if (iy == iyStop) {
                        return false;
                    }
                }
                else {
                    if (hitInsideCell(ray, objectIntersectionInfo, cell) && objectIntersectionInfo.t < tzNext) {
                        return true;
                    }

                    tzNext += dtz;
                    iz += izStep;

                    if (iz == izStop) {
                        return false;
                    }
                }
            }
        }
    }

    bool shadowHit(const Ray& ray, float& IntersectionT) const {
        float ox = ray.origin.x;
        float oy = ray.origin.y;
        float oz = ray.origin.z;
        float dx = ray.direction.x;
        float dy = ray.direction.y;
        float dz = ray.direction.z;

        float x0 = boundingBox.x0;
        float y0 = boundingBox.y0;
        float z0 = boundingBox.z0;
        float x1 = boundingBox.x1;
        float y1 = boundingBox.y1;
        float z1 = boundingBox.z1;

        float txMin, tyMin, tzMin;
        float txMax, tyMax, tzMax;

        float a = 1.0f / dx;
        if (a >= 0.0f) {
            txMin = (x0 - ox) * a;
            txMax = (x1 - ox) * a;
        }
        else {
            txMin = (x1 - ox) * a;
            txMax = (x0 - ox) * a;
        }

        float b = 1.0f / dy;
        if (b >= 0.0f) {
            tyMin = (y0 - oy) * b;
            tyMax = (y1 - oy) * b;
        }
        else {
            tyMin = (y1 - oy) * b;
            tyMax = (y0 - oy) * b;
        }

        float c = 1.0f / dz;
        if (c >= 0.0f) {
            tzMin = (z0 - oz) * c;
            tzMax = (z1 - oz) * c;
        }
        else {
            tzMin = (z1 - oz) * c;
            tzMax = (z0 - oz) * c;
        }

        float t0, t1;

        if (txMin > tyMin) {
            t0 = txMin;
        }
        else {
            t0 = tyMin;
        }

        if (tzMin > t0) {
            t0 = tzMin;
        }

        if (txMax < tyMax) {
            t1 = txMax;
        }
        else {
            t1 = tyMax;
        }

        if (tzMax < t1) {
            t1 = tzMax;
        }

        if (t0 > t1) {
            return false;
        }

        // Initial cell coordinates

        int ix, iy, iz;

        if (boundingBox.inside(ray.origin)) {  			// Does the ray start inside the grid?
            ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
            iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
            iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
        }
        else {
            glm::vec3 p = ray.origin + t0 * ray.direction;         // Initial hit point with grid's bounding box
            ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
            iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
            iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
        }

        // Ray parameter increments per cell in the x, y, and z directions

        float dtx = (txMax - txMin) / nx;
        float dty = (tyMax - tyMin) / ny;
        float dtz = (tzMax - tzMin) / nz;

        float 	txNext, tyNext, tzNext;
        int 	ixStep, iyStep, izStep;
        int 	ixStop, iyStop, izStop;

        if (dx > 0.0f) {
            txNext = txMin + (ix + 1) * dtx;
            ixStep = +1;
            ixStop = nx;
        }
        else {
            txNext = txMin + (nx - ix) * dtx;
            ixStep = -1;
            ixStop = -1;
        }

        if (dx == 0.0f) {
            txNext = FLT_MAX;
            ixStep = -1;
            ixStop = -1;
        }


        if (dy > 0.0f) {
            tyNext = tyMin + (iy + 1) * dty;
            iyStep = +1;
            iyStop = ny;
        }
        else {
            tyNext = tyMin + (ny - iy) * dty;
            iyStep = -1;
            iyStop = -1;
        }

        if (dy == 0.0) {
            tyNext = FLT_MAX;
            iyStep = -1;
            iyStop = -1;
        }

        if (dz > 0) {
            tzNext = tzMin + (iz + 1) * dtz;
            izStep = +1;
            izStop = nz;
        }
        else {
            tzNext = tzMin + (nz - iz) * dtz;
            izStep = -1;
            izStop = -1;
        }

        if (dz == 0.0) {
            tzNext = FLT_MAX;
            izStep = -1;
            izStop = -1;
        }

        // Traverse the grid

        while (true) {
            const std::vector<Object *>& cell = cells[ix + nx * iy + nx * ny * iz];

            if (txNext < tyNext && txNext < tzNext) {
                if (shadowHitInsideCell(ray, IntersectionT, cell) && IntersectionT < txNext) {
                    return true;
                }

                txNext += dtx;
                ix += ixStep;

                if (ix == ixStop) {
                    return false;
                }
            }
            else {
                if (tyNext < tzNext) {
                    if (shadowHitInsideCell(ray, IntersectionT, cell) && IntersectionT < tyNext) {
                        return true;
                    }

                    tyNext += dty;
                    iy += iyStep;

                    if (iy == iyStop) {
                        return false;
                    }
                }
                else {
                    if (shadowHitInsideCell(ray, IntersectionT, cell) && IntersectionT < tzNext) {
                        return true;
                    }

                    tzNext += dtz;
                    iz += izStep;

                    if (iz == izStop) {
                        return false;
                    }
                }
            }
        }
    }

    BoundingBox getBoundingBox() const {
        return boundingBox;
    }

private:
    bool hitInsideCell(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo, const std::vector<Object *>& cell) const {
        bool result = false;
        ObjectIntersectionInfo lastIntersectionInfo;
        float tMin = FLT_MAX;

        for (auto object = cell.begin(); object != cell.end(); ++object) {
            if ((*object)->hit(ray, lastIntersectionInfo) && lastIntersectionInfo.t < tMin) {
                result = true;
                objectIntersectionInfo = lastIntersectionInfo;
                tMin = lastIntersectionInfo.t;
            }
        }

        return result;
    }
    bool shadowHitInsideCell(const Ray& ray, float& IntersectionT, const std::vector<Object *>& cell) const {
        bool result = false;
        float lastT;
        float tMin = FLT_MAX;

        for (auto object = cell.begin(); object != cell.end(); ++object) {
            if ((*object)->shadowHit(ray, lastT) && lastT < tMin) {
                result = true;
                IntersectionT = lastT;
                tMin = lastT;
            }
        }

        return result;
    }

    float clamp(const float x, const float min, const float max) const {
        return x < min ? min : (x > max ? max : x);
    }

    void prepareGrid(const std::vector<Object *> objects) {
        // Find the minimum and maximum coordinates of the grid

        glm::vec3 p0 = findMinBounds(objects);
        glm::vec3 p1 = findMaxBounds(objects);
        
        boundingBox.x0 = p0.x;
        boundingBox.y0 = p0.y;
        boundingBox.z0 = p0.z;
        boundingBox.x1 = p1.x;
        boundingBox.y1 = p1.y;
        boundingBox.z1 = p1.z;
        
        // Compute the number of grid cells in the x, y, and z directions
        
        int numObjects = objects.size();
        
        // Dimensions of the grid in the x, y, and z directions
        
        float wx = p1.x - p0.x;
        float wy = p1.y - p0.y;
        float wz = p1.z - p0.z;
        
        float multiplier = 2.0f;  	// multiplyer scales the number of grid cells relative to the number of objects
        
        float s = pow(wx * wy * wz / numObjects, 0.3333333f);
        nx = multiplier * wx / s + 1;
        ny = multiplier * wy / s + 1;
        nz = multiplier * wz / s + 1;
        
        // Set up the array of grid cells with null pointers
        
        int numCells = nx * ny * nz;
        cells.reserve(numCells);
        
        for (int j = 0; j < numCells; j++) {
            cells.push_back(std::vector<Object *>());
        }
        
        // Put the objects into the cells
        
        for (int j = 0; j < numObjects; j++) {
            BoundingBox objBoundingBox = objects[j]->getBoundingBox();
        
            // Compute the cell indices at the corners of the bounding box of the object
        
            int ixmin = clamp((objBoundingBox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
            int iymin = clamp((objBoundingBox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
            int izmin = clamp((objBoundingBox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
            int ixmax = clamp((objBoundingBox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
            int iymax = clamp((objBoundingBox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
            int izmax = clamp((objBoundingBox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
        
            // Add the object to the cells
        
            for (int iz = izmin; iz <= izmax; iz++) {
                for (int iy = iymin; iy <= iymax; iy++) {
                    for (int ix = ixmin; ix <= ixmax; ix++) {
        
                        int index = ix + nx * iy + nx * ny * iz;
                        cells[index].push_back(objects[j]);
                    }
                }
            }
        }
    }

    glm::vec3 findMinBounds(const std::vector<Object *> objects) const {
        glm::vec3 result(FLT_MAX, FLT_MAX, FLT_MAX);

        for (int j = 0; j < objects.size(); j++) {
            BoundingBox objBoundingBox = objects[j]->getBoundingBox();
            
            if (objBoundingBox.x0 < result.x) {
                result.x = objBoundingBox.x0;
            }
            if (objBoundingBox.y0 < result.y) {
                result.y = objBoundingBox.y0;
            }
            if (objBoundingBox.z0 < result.z) {
                result.z = objBoundingBox.z0;
            }
        }

        result.x -= EPSILON;
        result.y -= EPSILON;
        result.z -= EPSILON;

        return result;
    }

    glm::vec3 findMaxBounds(const std::vector<Object *> objects) const {
        glm::vec3 result(FLT_MIN, FLT_MIN, FLT_MIN);

        for (int j = 0; j < objects.size(); j++) {
            BoundingBox objBoundingBox = objects[j]->getBoundingBox();
            
            if (objBoundingBox.x1 > result.x) {
                result.x = objBoundingBox.x1;
            }
            if (objBoundingBox.y1 > result.y) {
                result.y = objBoundingBox.y1;
            }
            if (objBoundingBox.z1 > result.z) {
                result.z = objBoundingBox.z1;
            }
        }

        result.x += EPSILON;
        result.y += EPSILON;
        result.z += EPSILON;

        return result;
    }

private:
    std::vector<std::vector<Object *>> cells;
    int nx, ny, nz;                      // Number of cells in the x, y, and z directions
    BoundingBox boundingBox;

private:
    static const float EPSILON;
};

template <class Object>
const float Grid<Object>::EPSILON = 0.001f;

#endif