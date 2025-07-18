#include "RenderObjects/Object2D.h"
#include "shaders/VertexShader.h"
#include "shaders/FragmentShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Object2D::Object2D()
{
    this->vertexAndColorData = nullptr;
    this->elementBufferData = nullptr;

    this->attributeHandle = 0;
    this->vcDataHandle = 0;
    this->elementHandle = 0;
    this->shaderProgramHandle = 0;

    this->dataSize = 0;
    this->elementSize = 0;
}

Object2D::Object2D(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize)
{
    this->vertexAndColorData = vcData;
    this->elementBufferData = elementData;
    this->dataSize = vcSize;
    this->elementSize = eSize;
}

Object2D::~Object2D()
{
    if (this->vertexAndColorData)
        delete this->vertexAndColorData;
    if (this->elementBufferData)
        delete this->elementBufferData;

    if (this->attributeHandle != 0)
        glDeleteVertexArrays(1, &this->attributeHandle);
    if (this->vcDataHandle != 0)
        glDeleteBuffers(1, &this->vcDataHandle);
    if (this->elementHandle != 0)
        glDeleteBuffers(1, &this->elementHandle);
    if (this->shaderProgramHandle != 0)
        glDeleteProgram(this->shaderProgramHandle);
}

bool Object2D::compileShader()
{
    if (!vertexShader || !fragmentShader) {
        std::cout << "Shader source is null" << std::endl;
        return false;
    }

    // Compile vertex shader
    unsigned int vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderHandle, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderHandle);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderHandle, 512, nullptr, infoLog);
        std::cout << "Vertex Shader compilation failed: " << infoLog << std::endl;
        return false;
    }

    // Compile fragment shader
    unsigned int fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderHandle, 1, &fragmentShader, nullptr);
    glCompileShader(fragmentShaderHandle);
    glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderHandle, 512, nullptr, infoLog);
        std::cout << "Fragment Shader compilation failed: " << infoLog << std::endl;
        return false;
    }

    // Link shaders into program
    this->shaderProgramHandle = glCreateProgram();
    glAttachShader( this->shaderProgramHandle, vertexShaderHandle);
    glAttachShader( this->shaderProgramHandle, fragmentShaderHandle);
    glLinkProgram( this->shaderProgramHandle);
    glGetProgramiv( this->shaderProgramHandle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog( this->shaderProgramHandle, 512, nullptr, infoLog);
        std::cout << "Shader Program linking failed: " << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShaderHandle);
    glDeleteShader(fragmentShaderHandle);

    return true;
}

bool Object2D::buildGeometry()
{
    if (this->vertexAndColorData && this->dataSize > 0 && this->elementBufferData && this->elementSize > 0)
    {
        glGenVertexArrays(1, &this->attributeHandle);
        glGenBuffers(1, &this->vcDataHandle);
        glGenBuffers(1, &this->elementHandle);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after generating buffers" << std::endl;
            return false;
        }

        glBindVertexArray(this->attributeHandle);

        // Bind and fill interleaved vertices
        glBindBuffer(GL_ARRAY_BUFFER, this->vcDataHandle);
        glBufferData(GL_ARRAY_BUFFER, this->dataSize * sizeof(float), this->vertexAndColorData, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting VBO data" << std::endl;
            return false;
        }

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting position attribute" << std::endl;
            return false;
        }

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting color attribute" << std::endl;
            return false;
        }

        // Bind and fill element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementSize * sizeof(unsigned int), this->elementBufferData, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting EBO data" << std::endl;
            return false;
        }

        // Reset binds
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }
    else
        return false;
}

void Object2D::render()
{
    if (glfwGetCurrentContext() == nullptr) {
        std::cout << "No valid OpenGL context" << std::endl;
        return;
    }

    if (this->shaderProgramHandle != 0 && this->attributeHandle != 0 && this->vcDataHandle != 0 && this->elementHandle != 0)
    {
        glUseProgram(this->shaderProgramHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after use program" << std::endl;
        glBindVertexArray(this->attributeHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after bind VAO" << std::endl;
        glDrawElements(GL_TRIANGLES, this->elementSize, GL_UNSIGNED_INT, 0);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after draw" << std::endl;
    }
}