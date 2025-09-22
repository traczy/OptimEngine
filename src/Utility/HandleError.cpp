#include "Utility/HandleError.h"
#include "Utility/Logger.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

bool HandleError::printErrorIf(std::string prefix)
{
    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        switch (err) {
            case GL_NO_ERROR:
            {
                return false;
            }
            case GL_INVALID_ENUM:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Invalid enum"));
                return true;
            }
            case GL_INVALID_VALUE:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Invalid value"));
                return true;
            }
            case GL_INVALID_OPERATION:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Invalid operation"));
                return true;
            }
            case GL_STACK_OVERFLOW:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Stack overflow"));
                return true;
            }
            case GL_STACK_UNDERFLOW:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Stack underflow"));
                return true;
            }
            case GL_OUT_OF_MEMORY:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Out of memory"));
                return true;
            }
            case GL_INVALID_FRAMEBUFFER_OPERATION:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Invalid framebuffer operation"));
                return true;
            }
            default:
            {
                Logger::getInstance()->log(LogLevel::ERROR,  prefix.append(": Unknown error"));
                return true;
            }
        }
    }

    return false;
}
