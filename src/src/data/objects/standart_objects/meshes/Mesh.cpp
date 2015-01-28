#include "Mesh.h"

#include <string>
#include <algorithm>

#include "l3ds\l3ds.h"

#include "..\..\..\..\utils\RandGenerator.h"
#include "..\..\..\materials\Lambertian.h"
#include "..\..\..\materials\Emitting.h"
#include "..\..\..\materials\Phong.h"
#include "..\..\..\materials\Reflective.h"

using namespace std;

Mesh::Mesh(LMesh& mesh, L3DS& meshLoader) {
    // Create materials for mesh
    for (int i = 0; i < mesh.GetMaterialCount(); ++i) {
        int materialID = mesh.GetMaterial(i);
        loadMaterial(materialID, meshLoader);
    }

    for (int vertex = 0; vertex < mesh.GetVertexCount(); ++vertex) {
        const LVector4& currentVertex = mesh.GetVertex(vertex);
        vertices.push_back(glm::vec3(currentVertex.x, currentVertex.y, currentVertex.z));
        const LVector3& currentNormal = mesh.GetNormal(vertex);
        normals.push_back(glm::vec3(currentNormal.x, currentNormal.y, currentNormal.z));
    }

    for (int triangle = 0; triangle < mesh.GetTriangleCount(); ++triangle) {
        const LTriangle& currentTriangle = mesh.GetTriangle(triangle);
        const LTriangle2&  currentTriangle2 = mesh.GetTriangle2(triangle);
        
        triangles.push_back(new Triangle(*this, currentTriangle.a, currentTriangle.b, currentTriangle.c,
            currentTriangle2.materialId));
    }
}

Mesh::~Mesh() {
    for (auto triangle = triangles.begin(); triangle != triangles.end(); ++triangle) {
        delete *triangle;
    }
    for (auto material = materials.begin(); material != materials.end(); ++material) {
        delete material->second;
    }
}

const std::vector<glm::vec3>& Mesh::getVertices() const {
    return vertices;
}

const std::vector<glm::vec3>& Mesh::getNormals() const {
    return normals;
}

const Material * Mesh::getMaterial(int index) const {
    return materials.at(index);
}

void Mesh::loadMaterial(int materialID, L3DS& meshLoader) {
    LMaterial& lMaterial = meshLoader.GetMaterial(materialID);
    
    Material * material = nullptr;
    LColor3 diffuseColor;
    LColor3 specularColor;
    float shininess;
    switch (lMaterial.GetShadingType()) {
    // case sGouraud:
    case sFlat:
        diffuseColor = lMaterial.GetDiffuseColor();
        material = new Lambertian(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));
        break;
    case sGouraud:
    case sPhong:
        diffuseColor = lMaterial.GetDiffuseColor();
        specularColor = lMaterial.GetSpecularColor();
        shininess = lMaterial.GetShininess() * 9.0f + 5.0f;
        material = new Phong(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b),
            glm::vec3(specularColor.r, specularColor.g, specularColor.b), shininess);
        break;
    case sMetal:
        diffuseColor = lMaterial.GetDiffuseColor();
        material = new Reflective(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));
        break;
    default:
        // Test
        cout << "Random shading model\n";
        material = new Lambertian(glm::vec3(RandGenerator::getRandGenerator().getNumberFromZeroToOne(),
                                            RandGenerator::getRandGenerator().getNumberFromZeroToOne(),
                                            RandGenerator::getRandGenerator().getNumberFromZeroToOne()));
        break;
    }

    materials[materialID] = material;
}

Triangle::Triangle(const Mesh& mesh, int aIndex, int bIndex, int cIndex, int materialIndex) : mesh(mesh) {

    this->aIndex = aIndex;
    this->bIndex = bIndex;
    this->cIndex = cIndex;

    this->materialIndex = materialIndex;
}

Triangle::~Triangle() {
}

static const float EPSILON = 0.001f;

bool Triangle::hit(const Ray& ray, ObjectIntersectionInfo& objectIntersectionInfo) const {
    const std::vector<glm::vec3>& vertices = mesh.getVertices();

    glm::vec3 v0(vertices[aIndex]);
    glm::vec3 v1(vertices[bIndex]);
    glm::vec3 v2(vertices[cIndex]);

    float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
    float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
    float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float invDenom = 1.0f / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * invDenom;

    if (beta < 0.0f) {
        return false;
    }

    float r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * invDenom;

    if (gamma < 0.0f) {
        return false;
    }

    if (beta + gamma > 1.0f) {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * invDenom;

    if (t < EPSILON) {
        return false;
    }

    objectIntersectionInfo.t = t;
    objectIntersectionInfo.normal = interpolateNormal(beta, gamma);
    objectIntersectionInfo.material = mesh.getMaterial(materialIndex);

    return true;
}

bool Triangle::shadowHit(const Ray& ray, float& IntersectionT) const {
    const std::vector<glm::vec3>& vertices = mesh.getVertices();

    glm::vec3 v0(vertices[aIndex]);
    glm::vec3 v1(vertices[bIndex]);
    glm::vec3 v2(vertices[cIndex]);

    float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
    float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
    float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float invDenom = 1.0f / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * invDenom;

    if (beta < 0.0f) {
        return false;
    }

    float r = r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * invDenom;

    if (gamma < 0.0f) {
        return false;
    }

    if (beta + gamma > 1.0f) {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * invDenom;

    if (t < EPSILON) {
        return false;
    }

    IntersectionT = t;

    return true;
}

BoundingBox Triangle::getBoundingBox() const {
    float delta = 0.0001f;
    const std::vector<glm::vec3>& vertices = mesh.getVertices();

    const glm::vec3& v1 = vertices[aIndex];
    const glm::vec3& v2 = vertices[bIndex];
    const glm::vec3& v3 = vertices[cIndex];

    return BoundingBox(min(min(v1.x, v2.x), v3.x) - delta, min(min(v1.y, v2.y), v3.y) - delta,
                       min(min(v1.z, v2.z), v3.z) - delta, max(max(v1.x, v2.x), v3.x) + delta,
                       max(max(v1.y, v2.y), v3.y) + delta, max(max(v1.z, v2.z), v3.z) + delta);
}

glm::vec3 Triangle::interpolateNormal(const float beta, const float gamma) const {
    
    const std::vector<glm::vec3>& normals = mesh.getNormals();

    glm::vec3 result = (1.0f - beta - gamma) * normals[aIndex] + beta * normals[bIndex] + gamma * normals[cIndex];
    float length = sqrt(result.x * result.x + result.y * result.y + result.z * result.z);

    return result / length;
}