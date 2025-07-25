#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <vector>

class Camera;

class CameraController
{

public:
    static CameraController* instance;
    static CameraController* getInstance();

    void addCamera(Camera* cam);
    void setActiveCameraIndex(int index);

    Camera* getActiveCamera();

private:
    CameraController();

    std::vector<Camera*> cameras;
    int activeCameraIndex;

};

#endif // CAMERACONTROLLER_H