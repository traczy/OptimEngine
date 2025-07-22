#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <chrono>

#include "windowing/Mainwindow.h"
#include "RenderObjects/Object.h"

const int MainWindow::WIDTH = 800;
const int MainWindow::HEIGHT = 600;

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
    this->window = glfwCreateWindow(WIDTH, HEIGHT, "Optim", nullptr, nullptr);
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
    float* vertices = new float[40]{
        // Positions          // Texture Coords
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // Top-left
    };

    unsigned int* indices = new unsigned int[36]{
        0, 1, 2, 2, 3, 0, // Front
        1, 5, 6, 6, 2, 1, // Right
        5, 4, 7, 7, 6, 5, // Back
        4, 0, 3, 3, 7, 4, // Left
        3, 2, 6, 6, 7, 3, // Top
        4, 5, 1, 1, 0, 4  // Bottom
    };

    Object* obj = new Object(vertices, indices, 40, 36);
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
    if (!obj->loadTexture("C:\\Users\\jrbri\\Documents\\Megascans\\Downloaded\\surface\\Brick_Modern_ui5kaiqg\\ui5kaiqg_4K_Albedo.jpg"))
    {
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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