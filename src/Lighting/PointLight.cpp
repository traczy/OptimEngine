#include "Lighting/PointLight.h"

PointLight::PointLight()
{
    this->transform.setPosition(0.0f, 0.0f, 0.0f);
    setColor(1.0f, 1.0f, 1.0f);

    this->ambientStrength = 0.1f;
}

PointLight::PointLight(float x, float y, float z, float r, float g, float b)
{
    this->transform.setPosition(x, y, z);
    setColor(r, g, b);

    this->ambientStrength = 0.1f;
}

void PointLight::setLocation(float x, float y, float z)
{
    this->transform.setPosition(x, y, z);
}

void PointLight::setColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}