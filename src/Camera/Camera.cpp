#include "Camera/Camera.h"
#include "windowing/Mainwindow.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(MainWindow* context)
{
    setLocation(0.f, 0.f, 0.f);
    setFOVY(45.f);
    this->nearClip = 0.1f;
    this->farClip = 100.0f;
    this->context = context;

    this->projection = glm::perspective(glm::radians(this->fovY), (float)MainWindow::WIDTH / MainWindow::HEIGHT, this->nearClip, this->farClip);
    this->view = glm::translate(glm::mat4(1.0f), glm::vec3(this->x, this->y, this->z));
}

Camera::Camera(MainWindow* context, float x, float y, float z, float fovY)
{
    setLocation(x, y, z);
    setFOVY(fovY);
    this->context = context;
}

void Camera::setLocation(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;

    // Update view matrix to new location
    this->view = glm::translate(glm::mat4(1.0f), glm::vec3(this->x, this->y, this->z)); 
}

void Camera::setFOVY(float fovY)
{
    this->fovY = fovY;

    // Update view projection to match
    this->projection = glm::perspective(glm::radians(this->fovY), (float)MainWindow::WIDTH / MainWindow::HEIGHT, this->nearClip, this->farClip);
}

void Camera::setNearClippingDistance(float nearClip)
{
    this->nearClip = nearClip;

    // Update view projection to match
    this->projection = glm::perspective(glm::radians(this->fovY), (float)MainWindow::WIDTH / MainWindow::HEIGHT, this->nearClip, this->farClip);
}

void Camera::setFarClippingDistance(float farClip)
{
    this->farClip = farClip;

    // Update view projection to match
    this->projection = glm::perspective(glm::radians(this->fovY), (float)MainWindow::WIDTH / MainWindow::HEIGHT, this->nearClip, this->farClip);   
}