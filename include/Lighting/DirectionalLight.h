#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

class DirectionalLight
{
public:
    DirectionalLight();

    void setDirection(const glm::vec3& dir) { lightDir = dir; }
    glm::vec3 getDirection() { return this->lightDir; }
    void setColor(const glm::vec3& color) { lightColor = color; }
    glm::vec3 getColor() { return this->lightColor; }

    void setAmbientStrength(const float& ambStr) { ambientStrength = ambStr; }
    float getAmbientStrength() { return this->ambientStrength; }

private:
    glm::vec3 lightDir;
    glm::vec3 lightColor;
    float ambientStrength;
};

#endif // DIRECTIONAL_LIGHT_H