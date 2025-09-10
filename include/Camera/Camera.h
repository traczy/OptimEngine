#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Utility/Transform.h"

class MainWindow;

class Camera
{

public:
    Camera(MainWindow* context);
    Camera(MainWindow* context, float x, float y, float z, float fovY, float nearClip = 0.1f, float farCLip = 100.0f);

    void setLocation(float x, float y, float z);
    void setRotation(float pitch, float yaw, float roll);
    void setFOVY(float fovY);
    void setNearClippingDistance(float nearClip);
    void setFarClippingDistance(float farClip);

    glm::mat4 getProjection() { return this->projection; }
    glm::mat4 getView() { return this->view; }

    glm::vec3 getForwardVector() { return this->fv; }
    
    std::vector<float> getPosition() { return this->transform.getPosition(); }

private:
    Transform transform;
    float fovY;
    float nearClip;
    float farClip;

    MainWindow* context;
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 fv;

    void updateForwardVector();
};

#endif // CAMERA_H