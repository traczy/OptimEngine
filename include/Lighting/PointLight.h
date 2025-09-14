#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Utility/Transform.h"

class PointLight
{

public:
    PointLight();
    PointLight(float x, float y, float z, float r, float g, float b);

    std::vector<float> getPosition() { return this->transform.getPosition(); }

    float getRed() { return r; }
    float getGreen() { return g; }
    float getBlue() { return b; }

    void setLocation(float x, float y, float z);

    /*!
        r, g, and b should be a float between 0 and 1
    */
    void setColor(float r, float g, float b);

    void setAmbientStrength(const float& ambStr) { ambientStrength = ambStr; }
    float getAmbientStrength() { return this->ambientStrength; }

private:
    Transform transform;
    float x;
    float y;
    float z;

    float r;
    float g;
    float b;

    float ambientStrength;

};

#endif // POINTLIGHT_H