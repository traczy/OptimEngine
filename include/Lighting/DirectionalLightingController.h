#ifndef DIRECTIONAL_LIGHTING_CONTROLLER_H
#define DIRECTIONAL_LIGHTING_CONTROLLER_H

#include <vector>

class DirectionalLight;

class DirectionalLightingController
{

public:
    static DirectionalLightingController* getInstance();
    const std::vector<DirectionalLight*>& getLights() { return this->lights; }

    void addLight(DirectionalLight* light);

private:
    inline static DirectionalLightingController* instance = nullptr;

    std::vector<DirectionalLight*> lights;

    DirectionalLightingController();

};

#endif // DIRECTIONAL_LIGHTING_CONTROLLER_H