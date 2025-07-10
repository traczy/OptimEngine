#include "RenderObjects/Object2D.h"
#include "shaders/VertexShader.h"
#include "shaders/FragmentShader.h"

#include "glad/glad.h"
#include <iostream>

Object2D::Object2D()
{
    this->vertexAndColorData = nullptr;
    this->dataSize = 0;
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

    // TODO: Add protections to these deletions (toggle for indicating compiling of shaders and building of geometry has been done)
    glDeleteVertexArrays(1, &this->attributeHandle);
    glDeleteBuffers(1, &this->vcDataHandle);
    glDeleteBuffers(1, &this->elementHandle);
    glDeleteProgram(this->shaderProgramHandle);
}

bool Object2D::compileShader()
{
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

void Object2D::buildGeometry()
{
    if (this->vertexAndColorData && this->dataSize > 0)
    {
        glGenVertexArrays(1, &this->attributeHandle);
        glGenBuffers(1, &this->vcDataHandle);
        glGenBuffers(1, &this->elementHandle);

        glBindVertexArray(this->attributeHandle);

        // Bind and fill interleaved vertices
        glBindBuffer(GL_ARRAY_BUFFER, this->vcDataHandle);
        glBufferData(GL_ARRAY_BUFFER, this->dataSize * sizeof(float), this->vertexAndColorData, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Bind and fill element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementSize * sizeof(unsigned int), this->elementBufferData, GL_STATIC_DRAW);

        // Reset binds
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Object2D::render()
{
    if (this->vertexAndColorData && this->dataSize > 0)
    {
        glUseProgram(this->shaderProgramHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after use program" << std::endl;
        glBindVertexArray(this->attributeHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after bind VAO" << std::endl;
        glDrawElements(GL_TRIANGLES, this->elementSize, GL_UNSIGNED_INT, 0);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after draw" << std::endl;
    }
}