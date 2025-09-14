#include "Lighting/DirectionalLightingController.h"
#include "Lighting/DirectionalLight.h"

DirectionalLightingController::DirectionalLightingController()
{

}

DirectionalLightingController* DirectionalLightingController::getInstance()
{
    if (!instance)
        instance = new DirectionalLightingController();

    return instance;
}

void DirectionalLightingController::addLight(DirectionalLight* light)
{
    this->lights.push_back(light);
}