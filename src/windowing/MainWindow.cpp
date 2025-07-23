#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <chrono>

#include "windowing/Mainwindow.h"
#include "RenderObjects/Object.h"
#include "Lighting/PointLight.h"

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
    float* vertices = new float[120]{
        // Positions          // Texture Coords
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left
        // Back face
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-right
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-left
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-right
        // Right face
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-front
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-back
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-back
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-front
        // Left face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-back
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-front
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-front
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-back
        // Top face
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Front-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Front-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Back-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Back-left
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Back-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Back-right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // Front-right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f  // Front-left
    };

    // Tangent vectors (3 floats per vertex)
    float* tangents = new float[144] {
        // Front face (normal: 0,0,1, tangent: 1,0,0, bitangent: 0,1,0)
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Top-right
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Top-left
        // Back face (normal: 0,0,-1, tangent: -1,0,0, bitangent: 0,1,0)
       -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
       -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
       -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Top-left
       -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Top-right
        // Right face (normal: 1,0,0, tangent: 0,0,-1, bitangent: 0,1,0)
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, // Bottom-front
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, // Bottom-back
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, // Top-back
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 0.0f, // Top-front
        // Left face (normal: -1,0,0, tangent: 0,0,1, bitangent: 0,1,0)
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Bottom-back
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Bottom-front
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-front
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Top-back
        // Top face (normal: 0,1,0, tangent: 1,0,0, bitangent: 0,0,-1)
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f,-1.0f, // Front-left
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f,-1.0f, // Front-right
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f,-1.0f, // Back-right
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f,-1.0f, // Back-left
        // Bottom face (normal: 0,-1,0, tangent: 1,0,0, bitangent: 0,0,1)
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Back-left
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Back-right
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Front-right
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f  // Front-left
    };

    unsigned int* indices = new unsigned int[36]{
        0,  1,  2,  2,  3,  0,  // Front
        4,  5,  6,  6,  7,  4,  // Back
        8,  9, 10, 10, 11,  8,  // Right
        12, 13, 14, 14, 15, 12,  // Left
        16, 17, 18, 18, 19, 16,  // Top
        20, 23, 22, 22, 21, 20   // Bottom (fixed)
    };

    Object* obj = new Object(vertices, indices, tangents, 120, 36, 144);
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
    if (!obj->loadTexture("C:\\Users\\jrbri\\Documents\\Megascans\\Downloaded\\surface\\Brick_Modern_ui5kaiqg\\ui5kaiqg_4K_Normal.jpg"))
    {
        delete obj;
        return;
    }

    PointLight* light = new PointLight(1.2f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f);
    PointLight* lightTwo = new PointLight(-1.2f, -1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
    obj->addAffectingLight(light);
    obj->addAffectingLight(lightTwo);

    auto begin = std::chrono::high_resolution_clock::now();
    size_t iters = 0;
    // Render loop
    while (!glfwWindowShouldClose(this->window))
    {
        // Input
        processInput();

        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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