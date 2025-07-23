#include "Lighting/PointLight.h"

PointLight::PointLight()
{
}

PointLight::PointLight(float x, float y, float z, float r, float g, float b)
{
    setLocation(x, y, z);
    setColor(r, g, b);
}

void PointLight::setLocation(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void PointLight::setColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}