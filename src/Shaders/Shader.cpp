#include "Shaders/Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <limits>
#include <iostream>

Shader::Shader()
{
    this->vertexSource = nullptr;
    this->fragmentSource = nullptr;
    this->programHandle = std::numeric_limits<unsigned int>::max();
}

Shader::~Shader()
{
    if (this->programHandle != std::numeric_limits<unsigned int>::max())
        glDeleteProgram(this->programHandle);
}

bool Shader::compile()
{
    // Don't recompile if already compiled
    if (this->programHandle == std::numeric_limits<unsigned int>::max())
    {
        // Create shader program, build on it throughout the function
        this->programHandle = glCreateProgram();

        // Compile vertex shader
        if (this->vertexSource)
        {
            unsigned int vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShaderHandle, 1, &this->vertexSource, nullptr);
            glCompileShader(vertexShaderHandle);

            // Error checking
            int success;
            char infoLog[512];
            glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShaderHandle, 512, nullptr, infoLog);
                std::cout << "Vertex Shader compilation failed: " << infoLog << std::endl;
                glDeleteShader(vertexShaderHandle);
                return false;
            }

            // Attach shader to program and cleanup
            glAttachShader(this->programHandle, vertexShaderHandle);
            glDeleteShader(vertexShaderHandle);
        }

        // Compile fragment shader
        if (this->fragmentSource)
        {
            unsigned int fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShaderHandle, 1, &this->fragmentSource, nullptr);
            glCompileShader(fragmentShaderHandle);

            // Error checking
            int success;
            char infoLog[512];
            glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShaderHandle, 512, nullptr, infoLog);
                std::cout << "Fragment Shader compilation failed: " << infoLog << std::endl;
                glDeleteShader(fragmentShaderHandle);
                return false;
            }

            // Attach shader to program and cleanup
            glAttachShader(this->programHandle, fragmentShaderHandle);
            glDeleteShader(fragmentShaderHandle);
        }

        // Final link of shader program
        int success;
        char infoLog[512];
        glLinkProgram( this->programHandle);
        glGetProgramiv( this->programHandle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog( this->programHandle, 512, nullptr, infoLog);
            std::cout << "Shader Program linking failed: " << infoLog << std::endl;
            return false;
        }
    }

    return true;
}