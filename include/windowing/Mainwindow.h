#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Camera/CameraController.h"
#include "Camera/Camera.h"
#include "Utility/Constants.h"

#include <glm/glm.hpp>

class GLFWwindow;

class MainWindow 
{
public:
    static const int WIDTH;
    static const int HEIGHT;

    static double mouseLastX;
    static double mouseLastY;
    static bool firstMouseCapture;

    MainWindow();

    bool isAlive();

    /*!
        Handles window resize
    */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    /**
     * @brief Responsible for handling the capturing of mouse movement and rotating
     *        the active camera based on that.
     * 
     * @param window - The window that the input is being captured on.
     * @param xpos - The current x-position for the mouse
     * @param ypos - The current y-position for the mouse
     */
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    /*!
        Starts render loop
    */
    void exec();

private:
    bool alive;
    GLFWwindow* window;

    /**
     * @brief Processes key input. Responsible for movement of active camera and closing the program via 'Esc'.
     * 
     * @param timeDelta - The time that has passed since the last frame.
     */
    void processInput(float timeDelta);

    /**
     * @brief Moves the active camera's position based on the parameters given.
     * 
     * @tparam F - Lambda to that will be utilized to move the position of the vector. Takes the position
     *             matrix of the camera and the vector to be applied to the matrix as parameters. Returns
     *             the new position matrix.
     * @param vec - The vector to be used to base the movement around. (up vector or right vector depening on the
     *              movement provided in the lambda)
     * @param operation - See F.
     */
    template<typename F>
    void moveActiveCamera(glm::vec3 vec, float timeDelta, F operation)
    {
        Camera* cam = CameraController::getInstance()->getActiveCamera();

        // Use forward vector to move forward in current facing direction
        std::vector<float> pos = cam->getPosition();
        glm::vec3 posMat = glm::vec3(pos[0], pos[1], pos[2]);
        posMat = operation(posMat, Constants::cameraMoveSpeed * timeDelta * glm::normalize(vec));
        cam->setLocation(posMat[0], posMat[1], posMat[2]);
    }
};

#endif // MAINWINDOW_H