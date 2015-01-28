#ifndef __CAMERA__
#define __CAMERA__

#include <string>
#include <vector>

#include "glm\glm.hpp"

class Camera {
public:
    void renderScene();

private:
    void saveImageToFile(const std::string fileName) const;

private:
    int viewPlaneWidth;
    int viewPlaneHeight;
    float viewPlaneDistance;
    int raysPerPixelAmount;
    glm::vec3 eyePosition;
    glm::vec3 lookAtPoint;
    glm::vec3 upVector;

    std::string resultingImageFileName;
    std::vector<glm::vec3> pixels;

public:
    static Camera& getCamera();
private:
    Camera();
    ~Camera();

    static Camera instance;
};

#endif