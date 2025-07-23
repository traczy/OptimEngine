#include "RenderObjects/Object.h"
#include "shaders/VertexShader.h"
#include "shaders/FragmentShader.h"
#include "windowing/Mainwindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>

Object::Object()
{
    this->vData = nullptr;
    this->elementBufferData = nullptr;
    this->tangentData = nullptr;

    this->attributeHandle = 0;
    this->vDataHandle = 0;
    this->elementHandle = 0;
    this->tangentHandle = 0;
    this->shaderProgramHandle = 0;

    this->dataSize = 0;
    this->elementSize = 0;
    this->tangentSize = 0;
}

Object::Object(float* vcData, unsigned int* elementData, float* tangentData, size_t vcSize, size_t eSize, size_t tangentSize)
{
    this->vData = vcData;
    this->elementBufferData = elementData;
    this->tangentData = tangentData;
    this->dataSize = vcSize;
    this->elementSize = eSize;
    this->tangentSize = tangentSize;

    this->attributeHandle = 0;
    this->vDataHandle = 0;
    this->elementHandle = 0;
    this->tangentHandle = 0;
    this->shaderProgramHandle = 0;
}

Object::~Object()
{
    if (this->vData)
        delete this->vData;
    if (this->elementBufferData)
        delete this->elementBufferData;
    if (this->tangentData)
        delete this->tangentData;

    if (this->attributeHandle != 0)
        glDeleteVertexArrays(1, &this->attributeHandle);
    if (this->vDataHandle != 0)
        glDeleteBuffers(1, &this->vDataHandle);
    if (this->elementHandle != 0)
        glDeleteBuffers(1, &this->elementHandle);
    if (this->tangentHandle != 0)
        glDeleteBuffers(1, &this->tangentHandle);
    if (this->shaderProgramHandle != 0)
        glDeleteProgram(this->shaderProgramHandle);

    for (unsigned int handle : this->textureHandles)
    {
        if (handle != 0)
            glDeleteTextures(1, &handle);
    }
}

bool Object::loadTexture(const char* path) {
    this->textureHandles.push_back(0);
    unsigned int* textureHandle = &this->textureHandles[this->textureHandles.size() - 1];
    glGenTextures(1, textureHandle);
    glBindTexture(GL_TEXTURE_2D, *textureHandle);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
        return false;
    }

    stbi_image_free(data);
    return true;
}

bool Object::compileShader()
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

bool Object::buildGeometry()
{
    if (this->vData && this->dataSize > 0 && this->elementBufferData && this->elementSize > 0)
    {
        glGenVertexArrays(1, &this->attributeHandle);
        glGenBuffers(1, &this->vDataHandle);
        glGenBuffers(1, &this->elementHandle);
        glGenBuffers(1, &this->tangentHandle);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after generating buffers" << std::endl;
            return false;
        }

        glBindVertexArray(this->attributeHandle);

        // Bind and fill interleaved vertices
        glBindBuffer(GL_ARRAY_BUFFER, this->vDataHandle);
        glBufferData(GL_ARRAY_BUFFER, this->dataSize * sizeof(float), this->vData, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting VBO data" << std::endl;
            return false;
        }

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting position attribute" << std::endl;
            return false;
        }

        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting UV attribute" << std::endl;
            return false;
        }

        // Setup tangent buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->tangentHandle);
        glBufferData(GL_ARRAY_BUFFER, this->tangentSize * sizeof(float), this->tangentData, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting tangent data" << std::endl;
            return false;
        }

        // Tangent attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting tangent attribute" << std::endl;
            return false;
        }

        // Bitangent attribute
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);
        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "GL Error after setting bi-tangent attribute" << std::endl;
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

void Object::render()
{
    if (glfwGetCurrentContext() == nullptr) {
        std::cout << "No valid OpenGL context" << std::endl;
        return;
    }

    if (this->shaderProgramHandle != 0 && this->attributeHandle != 0 && this->vDataHandle != 0 && this->elementHandle != 0)
    {
        glUseProgram(this->shaderProgramHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after use program" << std::endl;

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->textureHandles[0]);
        glUniform1i(glGetUniformLocation(this->shaderProgramHandle, "texture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->textureHandles[1]);
        glUniform1i(glGetUniformLocation(this->shaderProgramHandle, "normalMap"), 1);

        // Light properties
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 viewPos(0.0f, 0.0f, 3.0f);

        // Set lighting uniforms
        glUniform3fv(glGetUniformLocation(this->shaderProgramHandle, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(this->shaderProgramHandle, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(this->shaderProgramHandle, "viewPos"), 1, glm::value_ptr(viewPos));

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

        // TODO: Set to Universal Buffer Object
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)MainWindow::WIDTH / MainWindow::HEIGHT, 0.1f, 100.0f);

        // Pass matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramHandle, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramHandle, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramHandle, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(this->attributeHandle);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after bind VAO" << std::endl;

        glDrawElements(GL_TRIANGLES, this->elementSize, GL_UNSIGNED_INT, 0);
        if (glGetError() != GL_NO_ERROR) std::cout << "GL Error after draw" << std::endl;

    }
}