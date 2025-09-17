#include "Material/Material.h"
#include "Shaders/Shader.h"
#include "Utility/HandleError.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <limits>
#include <iostream>

BlinnPhongMaterial::BlinnPhongMaterial()
{
    this->shader = nullptr;
}

BlinnPhongMaterial::BlinnPhongMaterial(const std::vector<std::string>& texPaths)
{
    // Load all textures from files and place on GPU
    // TODO: This will be moved to some kind of texture streaming option eventually
    for (const std::string& path : texPaths)
        loadTexture(path);

    this->shader = nullptr;
}

BlinnPhongMaterial::~BlinnPhongMaterial()
{
    for (unsigned int handle : this->textureHandles)
    {
        if (handle != std::numeric_limits<unsigned int>::max())
            glDeleteTextures(1, &handle);
    }
}

void BlinnPhongMaterial::prepareRender()
{
    if (this->shader)
    {
        if (!this->shader->compile())
        {
            std::cout << "shader compilation failed" << std::endl;
            return;
        }
        
        unsigned int shaderHandle = this->shader->getProgram();
        if (shaderHandle != std::numeric_limits<unsigned int>::max())
        {
            // Set the shader program (if necessary)
            GLuint currentProgram;
            glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currentProgram);
            if (currentProgram != shaderHandle)
                glUseProgram(shaderHandle);

            if (this->textureHandles.size() < 4)
                std::cout << "Do not have necessary textures loaded for this material";

            // Bind textures in shader

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->textureHandles[0]);
            glUniform1i(glGetUniformLocation(shaderHandle, "diffuseMap"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, this->textureHandles[1]);
            glUniform1i(glGetUniformLocation(shaderHandle, "normalMap"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, this->textureHandles[2]);
            glUniform1i(glGetUniformLocation(shaderHandle, "specMap"), 2);
            
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, this->textureHandles[3]);
            glUniform1i(glGetUniformLocation(shaderHandle, "roughnessMap"), 3);
        }
    }
}

bool BlinnPhongMaterial::loadTexture(const std::string& texPath)
{
    this->textureHandles.push_back(std::numeric_limits<unsigned int>::max());
    unsigned int* textureHandle = &this->textureHandles[this->textureHandles.size() - 1];
    glGenTextures(1, textureHandle);
    glBindTexture(GL_TEXTURE_2D, *textureHandle);

    // Set texture parameters
    // TODO: Break these out into configurable values for each material
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically
    unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        HandleError::printErrorIf("After tex image 2d");
        glGenerateMipmap(GL_TEXTURE_2D);
        HandleError::printErrorIf("After generate mipmap");
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << texPath << std::endl;
        std::flush(std::cout);
        return false;
    }

    return true;
}