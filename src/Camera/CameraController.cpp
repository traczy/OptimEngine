#include "Camera/CameraController.h"
#include "Camera/Camera.h"

CameraController* CameraController::instance = nullptr;

CameraController::CameraController()
{
    this->activeCameraIndex = -1;
}

CameraController* CameraController::getInstance()
{
    if (!instance)
    {
        instance = new CameraController();
    }

    return instance;
}

void CameraController::addCamera(Camera* cam)
{
    if (cam)
    {
        this->cameras.push_back(cam);

        if (this->cameras.size() == 1)
            this->activeCameraIndex = 0;
    }
}

void CameraController::setActiveCameraIndex(int index)
{
    if (index >= 0 && index < this->cameras.size())
        this->activeCameraIndex = index;
}

Camera* CameraController::getActiveCamera()
{
    if (this->activeCameraIndex != -1)
        return this->cameras[this->activeCameraIndex];
}