#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "windowing/Mainwindow.h"
#include <cmath>
#include <chrono>

MainWindow::MainWindow()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        this->alive = false;
        return;
    }

    // Set GLFW window hints for OpenGL version 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    this->window = glfwCreateWindow(800, 600, "Optim", nullptr, nullptr);
    if (!this->window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        this->alive = false;
        return;
    }

    // Make context current
    glfwMakeContextCurrent(this->window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        this->alive = false;
        return;
    }

    // Set viewport and callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);

    this->alive = true;
}

bool MainWindow::isAlive()
{
    return this->alive;
}

void MainWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MainWindow::processInput()
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
}

void MainWindow::exec()
{
    auto begin = std::chrono::high_resolution_clock::now();
    size_t iters = 0;
    // Render loop
    while (!glfwWindowShouldClose(this->window))
    {
        // Input
        processInput();

        // Rendering
        float time = (float)glfwGetTime();
        float greenValue = (sin(time) / 2.0f) + 0.5f;
        
        glClearColor(0.2f, greenValue, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll events
        glfwSwapBuffers(this->window);
        glfwPollEvents();

        iters++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double fps = (double)iters / (double)std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    std::cout << fps << std::endl;

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    this->alive = false;
}