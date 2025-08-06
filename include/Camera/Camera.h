#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Utility/Transform.h"

class MainWindow;

class Camera
{

public:
    Camera(MainWindow* context);
    Camera(MainWindow* context, float x, float y, float z, float fovY);

    void setLocation(float x, float y, float z);
    void setRotation(float pitch, float yaw, float roll);
    void setFOVY(float fovY);
    void setNearClippingDistance(float nearClip);
    void setFarClippingDistance(float farClip);

    glm::mat4 getProjection() { return this->projection; }
    glm::mat4 getView() { return this->view; }

private:
    Transform transform;
    float fovY;
    float nearClip;
    float farClip;

    MainWindow* context;
    glm::mat4 projection;
    glm::mat4 view;

};

#endif // CAMERA_H