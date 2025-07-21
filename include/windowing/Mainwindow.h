#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class GLFWwindow;

class MainWindow 
{
public:
    static const int WIDTH;
    static const int HEIGHT;

    MainWindow();

    bool isAlive();

    /*!
        Handles window resize
    */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    /*!
        Starts render loop
    */
    void exec();

private:
    bool alive;
    GLFWwindow* window;

    void processInput();
};

#endif // MAINWINDOW_H