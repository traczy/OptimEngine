#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class MainWindow;

class Camera
{

public:
    Camera(MainWindow* context);
    Camera(MainWindow* context, float x, float y, float z, float fovY);

    void setLocation(float x, float y, float z);
    void setFOVY(float fovY);
    void setNearClippingDistance(float nearClip);
    void setFarClippingDistance(float farClip);

    glm::mat4 getProjection() { return this->projection; }
    glm::mat4 getView() { return this->view; }

private:
    float x;
    float y;
    float z;
    float fovY;
    float nearClip;
    float farClip;

    MainWindow* context;
    glm::mat4 projection;
    glm::mat4 view;

};

#endif // CAMERA_H