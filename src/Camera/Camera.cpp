#include "Camera/Camera.h"
#include "windowing/Mainwindow.h"
#include "Utility/Constants.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(MainWindow* context)
{
    setFOVY(45.f);
    this->nearClip = 0.1f;
    this->farClip = 100.0f;
    this->context = context;

    this->projection = glm::perspective(glm::radians(this->fovY), (float)MainWindow::WIDTH / MainWindow::HEIGHT, this->nearClip, this->farClip);

    updateForwardVector();

    std::vector<float> position = this->transform.getPosition();
    this->view = glm::lookAt(glm::vec3(position[0], position[1], position[2]), this->fv, Constants::upVector);
}

Camera::Camera(MainWindow* context, float x, float y, float z, float fovY, float nearClip, float farClip)
{
    setLocation(x, y, z);

    this->nearClip = nearClip;
    this->farClip = farClip;
    setFOVY(fovY);

    this->context = context;

    updateForwardVector();
}

void Camera::setLocation(float x, float y, float z)
{
    this->transform.setPosition(x, y, z);

    // Update view matrix to new location
    glm::vec3 pos = { x, y, z };
    this->view = glm::lookAt(pos, pos + this->fv, Constants::upVector);
}

void Camera::setRotation(float pitch, float yaw, float roll)
{
    this->transform.setRotation(pitch, yaw, roll);

    updateForwardVector();

    auto p = this->transform.getPosition();
    glm::vec3 pos = { p[0], p[1], p[2] };
    this->view = glm::lookAt(pos, pos + this->fv, Constants::upVector);
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

void Camera::updateForwardVector()
{
    std::vector<float> rotation = this->transform.getRotation();

    glm::vec3 direction;
    direction.x = cos(glm::radians(rotation[1])) * cos(glm::radians(rotation[0]));
    direction.y = sin(glm::radians(rotation[0]));
    direction.z = sin(glm::radians(rotation[1])) * cos(glm::radians(rotation[0]));

    this->fv = glm::normalize(direction);
}