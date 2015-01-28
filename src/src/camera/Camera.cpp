#include "Camera.h"

#include <string>
#include <iostream>

#include "jsoncons\json.hpp"
#include "atlimage.h"

#include "..\tracer\TracerFactory.h"
#include "..\utils\Ray.h"
#include "..\utils\RandGenerator.h"

using namespace std;
using namespace jsoncons;

Camera Camera::instance = Camera();

Camera::Camera() {
    string configFileName = "resources/configurations/CameraConfiguration.json";

    json configFile = json::parse_file(configFileName);
    try {
        json screenResolution = configFile["screenResolution"];
        this->viewPlaneWidth = screenResolution["width"].as<int>();
        this->viewPlaneHeight = screenResolution["height"].as<int>();

        this->viewPlaneDistance = this->viewPlaneHeight / 2.0f;

        this->raysPerPixelAmount = configFile["raysPerPixelAmount"].as<int>();

        json eyePosition = configFile["eyePosition"];
        this->eyePosition = glm::vec3(eyePosition["x"].as<double>(), eyePosition["y"].as<double>(), eyePosition["z"].as<double>());

        json lookAtPoint = configFile["lookAtPoint"];
        this->lookAtPoint = glm::vec3(lookAtPoint["x"].as<double>(), lookAtPoint["y"].as<double>(), lookAtPoint["z"].as<double>());

        json upVector = configFile["upVector"];
        this->upVector = glm::vec3(upVector["x"].as<double>(), upVector["y"].as<double>(), upVector["z"].as<double>());

        this->resultingImageFileName = configFile["resultingImageFileName"].as<string>();

    } catch (const json_exception& e) {
        cerr << "Unable to read " + configFileName << endl;
        cerr << e.what() << endl;
    }
}

Camera::~Camera() {
}

void Camera::renderScene() {
    pixels.resize(viewPlaneHeight * viewPlaneWidth);

    // Compute camera coordinate system (viewDirection, right, up vectors)
    glm::vec3 viewDirection = glm::normalize(lookAtPoint - eyePosition);
    glm::vec3 right = glm::cross(viewDirection, glm::normalize(upVector));
    if (glm::distance(right, glm::vec3(0.0f, 0.0f, 0.0f)) < 0.001f) {
        throw std::string("up vector is parallel to viewDirection vector of the camera");
    }
    glm::vec3 up = glm::cross(right, viewDirection);

#pragma omp parallel for schedule(dynamic, 10)
    for (int currentHeight = (viewPlaneHeight / 2); currentHeight > (-viewPlaneHeight / 2); --currentHeight) {
        for (int currentWidth = (-viewPlaneWidth / 2); currentWidth < (viewPlaneWidth / 2); ++currentWidth) {
            glm::vec3 pixelColor = glm::vec3(0.0f, 0.0f, 0.0f);

            Ray ray;
            ray.origin = eyePosition;
            for (int i = 0; i < raysPerPixelAmount; ++i) {
                float deltaWidth = RandGenerator::getRandGenerator().getNumberFromZeroToOne();
                float deltaHeight = RandGenerator::getRandGenerator().getNumberFromZeroToOne();

                ray.direction = glm::normalize((currentWidth + deltaWidth) * right +
                    (currentHeight - deltaHeight) * up + viewPlaneDistance * viewDirection);
                pixelColor += TracerFactory::getTracerFactory().getTracer()->traceRay(ray, 1);
            }
            pixelColor /= raysPerPixelAmount;

            int index = (currentHeight + (viewPlaneHeight / 2) - 1) * viewPlaneWidth +
                (currentWidth - (-viewPlaneWidth / 2));
            pixels[index] = pixelColor;
        }
    }
    
    saveImageToFile(resultingImageFileName);
}

void Camera::saveImageToFile(const std::string fileName) const {
    CImage image;

    image.Create(viewPlaneWidth, viewPlaneHeight, 24);

    int pitch = image.GetPitch();
    unsigned char * imageBuffer = (unsigned char *)image.GetBits();

    if (pitch < 0) {
        imageBuffer += pitch * (viewPlaneHeight - 1);
        pitch = -pitch;
    }

    int i, j;
    int imageDisplacement = 0;
    int textureDisplacement = 0;

    for (i = 0; i < viewPlaneHeight; i++) {
        for (j = 0; j < viewPlaneWidth; j++) {
            glm::vec3 color = pixels[textureDisplacement + j];

            imageBuffer[imageDisplacement + j * 3] = static_cast<unsigned char>(glm::clamp(color.b, 0.0f, 1.0f) * 255.0f);     // R
            imageBuffer[imageDisplacement + j * 3 + 1] = static_cast<unsigned char>(glm::clamp(color.g, 0.0f, 1.0f) * 255.0f); // G
            imageBuffer[imageDisplacement + j * 3 + 2] = static_cast<unsigned char>(glm::clamp(color.r, 0.0f, 1.0f) * 255.0f); // B
        }

        imageDisplacement += pitch;
        textureDisplacement += viewPlaneWidth;
    }

    // Save image
    // wchar_t* wArr = new wchar_t[fileName.size() + 1];
    // for (size_t i = 0; i < fileName.size(); ++i) {
    //     wArr[i] = fileName[i];
    // }
    // wArr[fileName.size()] = 0;
    // image.Save(wArr);
    // image.Destroy();
    // delete[] wArr;

    image.Save(fileName.c_str());
    image.Destroy();
}

Camera& Camera::getCamera() {
    return instance;
}