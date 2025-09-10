#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    /*!
        Starts render loop
    */
    void exec();

private:
    bool alive;
    GLFWwindow* window;

    void processInput(float timeDelta);
};

#endif // MAINWINDOW_H