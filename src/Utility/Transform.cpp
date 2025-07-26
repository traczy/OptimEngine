#include "Utility/Transform.h"

Transform::Transform()
{
    setPosition(0.f, 0.f, 0.f);
    setRotation(0.f, 0.f, 0.f);
    setScale(1.f, 1.f, 1.f);
}

Transform::Transform(float x, float y, float z, float roll, float pitch, float yaw, float scaleX, float scaleY, float scaleZ)
{
    setPosition(x, y, z);
    setRotation(roll, pitch, yaw);
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

void Transform::setRotation(float roll, float pitch, float yaw)
{
    this->rotation = std::vector<float> { roll, pitch, yaw };
}

void Transform::setScale(float x, float y, float z)
{
    this->scale = std::vector<float> { x, y, z};
}