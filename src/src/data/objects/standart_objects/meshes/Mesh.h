#ifndef __MESH__
#define __MESH__

#include <vector>
#include <unordered_map>

#include "l3ds\l3ds.h"

#include "..\StandartObject.h"

class Triangle;

class Mesh : public StandartObject {
public:
    Mesh(LMesh& mesh, L3DS& meshLoader);
    virtual ~Mesh();

    virtual bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const = 0;

    virtual bool shadowHit(const Ray& ray, float& IntersectionT) const = 0;

    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const Material * getMaterial(int index) const;

protected:
    std::vector<Triangle *> triangles;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::unordered_map<int, const Material *> materials;

private:
    void loadMaterial(int materialID, L3DS& meshLoader);
};

class Triangle {
public:
    Triangle(const Mesh& mesh, int aIndex, int bIndex, int cIndex, int materialIndex);
    ~Triangle();

    bool hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const;

    bool shadowHit(const Ray& ray, float& IntersectionT) const;

    BoundingBox getBoundingBox() const;

private:
    glm::vec3 interpolateNormal(const float beta, const float gamma) const;

private:
    const Mesh& mesh;

    int aIndex;
    int bIndex;
    int cIndex;

    int materialIndex;
};

#endif