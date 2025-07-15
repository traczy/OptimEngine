#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <chrono>

#include "windowing/Mainwindow.h"
#include "RenderObjects/Object2D.h"

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "GL DEBUG: " << message << std::endl;
}

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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

    if (glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
    }
    else
    {
        std::cout << "GLDebug output not supported" << std::endl;
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
    // Define interleaved vertices with positions and colors (using double)
    float* vertices = new float[24]{
        // Positions         // Colors
        -0.5, -0.5, 0.0,    1.0, 0.0, 0.0, // Bottom-left (Red)
         0.5, -0.5, 0.0,    0.0, 1.0, 0.0, // Bottom-right (Green)
         0.5,  0.5, 0.0,    0.0, 0.0, 1.0, // Top-right (Blue)
        -0.5,  0.5, 0.0,    1.0, 1.0, 0.0  // Top-left (Yellow)
    };

    unsigned int* indices = new unsigned int[6]{
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    Object2D* obj = new Object2D(vertices, indices, 24, 6);
    if (!obj->compileShader())
    {
        std::cout << "error compiling shaders" << std::endl;
        delete obj;
        return;
    }
    if (!obj->buildGeometry())
    {
        std::cout << "error building geometry" << std::endl;
        delete obj;
        return;
    }

    auto begin = std::chrono::high_resolution_clock::now();
    size_t iters = 0;
    // Render loop
    while (!glfwWindowShouldClose(this->window))
    {
        // Input
        processInput();

        // Rendering
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after clear" << std::endl;

        obj->render();

        // Swap buffers and poll events
        glfwSwapBuffers(this->window);
        if (!glfwWindowShouldClose(this->window))
            glfwPollEvents();
        else
            std::cout << "should close" << std::endl;
        const char* glfwError;
        if (glfwGetError(&glfwError) != GLFW_NO_ERROR) {
            std::cout << "GLFW Error: " << glfwError << std::endl;
        }

        iters++;
    }

    delete obj;

    auto end = std::chrono::high_resolution_clock::now();
    double fps = (double)iters / (double)std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    std::cout << fps << std::endl;

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    this->alive = false;
}