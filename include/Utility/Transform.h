#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

class Transform
{

public:
    Transform();
    Transform(float x, float y, float z, float roll, float pitch, float yaw, float scaleX, float scaleY, float scaleZ);
    Transform(std::vector<float>& pos, std::vector<float>& rot, std::vector<float>& scale);

    std::vector<float> getPosition() { return this->position; }
    std::vector<float> getRotation() { return this->rotation; }
    std::vector<float> getScale() { return this->scale; }

    void setPosition(float x, float y, float z);
    void setRotation(float roll, float pitch, float yaw);
    void setScale(float x, float y, float z);

private:
    std::vector<float> position;
    std::vector<float> rotation;
    std::vector<float> scale;

};

#endif // TRANSFORM_H