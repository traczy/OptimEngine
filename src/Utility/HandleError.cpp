#include "Utility/HandleError.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void HandleError::printErrorIf(std::string prefix)
{
    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cout << "GL Error after bind VAO" << std::endl;
        switch (err) {
            case GL_NO_ERROR:
            {
                break;
            }
            case GL_INVALID_ENUM:
            {
                std::cout << prefix << ": Invalid enum" << std::endl;
                break;
            }
            case GL_INVALID_VALUE:
            {
                std::cout << prefix << ": Invalid value" << std::endl;
                break;
            }
            case GL_INVALID_OPERATION:
            {
                std::cout << prefix << ": Invalid operation" << std::endl;
                break;
            }
            case GL_STACK_OVERFLOW:
            {
                std::cout << prefix << ": Stack overflow" << std::endl;
                break;
            }
            case GL_STACK_UNDERFLOW:
            {
                std::cout << prefix << ": Stack underflow" << std::endl;
                break;
            }
            case GL_OUT_OF_MEMORY:
            {
                std::cout << prefix << ": Out of memory" << std::endl;
                break;
            }
            case GL_INVALID_FRAMEBUFFER_OPERATION:
            {
                std::cout << prefix << ": Invalid framebuffer operation" << std::endl;
                break;
            }
            default:
            {
                std::cout << prefix << ": Unknown error" << std::endl;
                break;
            }
        }
    }
}