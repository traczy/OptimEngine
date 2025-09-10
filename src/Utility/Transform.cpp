#include "Utility/Transform.h"

Transform::Transform()
{
    setPosition(0.f, 0.f, 0.f);
    setRotation(0.f, 0.f, 0.f);
    setScale(1.f, 1.f, 1.f);
}

Transform::Transform(float x, float y, float z, float pitch, float yaw, float roll, float scaleX, float scaleY, float scaleZ)
{
    setPosition(x, y, z);
    setRotation(pitch, yaw, roll);
    setScale(scaleX, scaleY, scaleZ);
}

Transform::Transform(std::vector<float>& pos, std::vector<float>& rot, std::vector<float>& scale)
{
    this->position = pos;
    this->position = rot;
    this->position = scale;
}

void Transform::setPosition(float x, float y, float z)
{
    this->position = std::vector<float> { x, y, z };
}

void Transform::setRotation(float pitch, float yaw, float roll)
{
    this->rotation = std::vector<float> { pitch, yaw, roll };
}

void Transform::setScale(float x, float y, float z)
{
    this->scale = std::vector<float> { x, y, z};
}