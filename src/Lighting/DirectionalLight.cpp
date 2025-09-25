#include "Lighting/DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
    setColor(glm::vec3{ 1.0f, 1.0f, 1.0f });
    setDirection(glm::vec3{ 0.0f, -1.0f, 0.0f });
    setAmbientStrength(0.0f);
}
