#include "RenderObjects/Object.h"
#include "Shaders/Shader.h"
#include "Windowing/Mainwindow.h"
#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"
#include "Lighting/DirectionalLightingController.h"
#include "Camera/CameraController.h"
#include "Camera/Camera.h"
#include "Material/Material.h"
#include "Utility/HandleError.h"
#include "Utility/Logger.h"

#include <Glad/glad.h>
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
    this->material = nullptr;

    this->attributeHandle = 0;
    this->vDataHandle = 0;
    this->elementHandle = 0;
    this->tangentHandle = 0;

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
    this->material = nullptr;

    this->attributeHandle = 0;
    this->vDataHandle = 0;
    this->elementHandle = 0;
    this->tangentHandle = 0;
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

    for (PointLight* light : this->affectingPointLights)
        delete light;
}

void Object::addAffectingLight(PointLight* light)
{
    this->affectingPointLights.push_back(light);
}

bool Object::buildGeometry()
{
    if (this->vData && this->dataSize > 0 && this->elementBufferData && this->elementSize > 0)
    {
        glGenVertexArrays(1, &this->attributeHandle);
        glGenBuffers(1, &this->vDataHandle);
        glGenBuffers(1, &this->elementHandle);
        glGenBuffers(1, &this->tangentHandle);
        if (HandleError::printErrorIf("GL Error after generating buffers"))
            return false;

        glBindVertexArray(this->attributeHandle);

        // Bind and fill interleaved vertices
        glBindBuffer(GL_ARRAY_BUFFER, this->vDataHandle);
        glBufferData(GL_ARRAY_BUFFER, this->dataSize * sizeof(float), this->vData, GL_STATIC_DRAW);
        if (HandleError::printErrorIf("GL Error after setting VBO data"))
            return false;

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        if (HandleError::printErrorIf("GL Error after setting position attribute"))
            return false;

        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        if (HandleError::printErrorIf("GL Error after setting UV attribute"))
            return false;

        // Setup tangent buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->tangentHandle);
        glBufferData(GL_ARRAY_BUFFER, this->tangentSize * sizeof(float), this->tangentData, GL_STATIC_DRAW);
        if (HandleError::printErrorIf("GL Error after setting tangent data"))
            return false;

        // Tangent attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        if (HandleError::printErrorIf("GL Error after setting tangent attribute"))
            return false;

        // Bitangent attribute
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);
        if (HandleError::printErrorIf("GL Error after setting bi-tangent attribute"))
            return false;

        // Bind and fill element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementSize * sizeof(unsigned int), this->elementBufferData, GL_STATIC_DRAW);
        if (HandleError::printErrorIf("GL Error after setting EBO data"))
            return false;

        // Reset binds
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        if (HandleError::printErrorIf("GL Error after resetting binds"))
            return false;

        return true;
    }
    else
        return false;
}

void Object::render()
{
    if (glfwGetCurrentContext() == nullptr)
    {
        Logger::getInstance()->log(LogLevel::ERROR, "No valid OpenGL context");
        return;
    }

    if (this->material && this->attributeHandle != 0 && this->vDataHandle != 0 && this->elementHandle != 0)
    {
        this->material->prepareRender();

        setPointLightingInShader();
        setDirectionalLightingInShader();

        // Scale, rotate, then translate
        std::vector<float> scale = this->transform.getScale();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(scale[0], scale[1], scale[2]));

        // Apply transform rotation first, then animate model by time (rotating based on the time)
        std::vector<float> rot = this->transform.getRotation();
        model = glm::rotate(model, glm::radians(rot[1]), glm::vec3(0.f, 1.f, 0.f)); // Yaw rotation
        model = glm::rotate(model, glm::radians(rot[0]), glm::vec3(1.f, 0.f, 0.f)); // Pitch rotation
        model = glm::rotate(model, glm::radians(rot[2]), glm::vec3(0.f, 0.f, 1.f)); // Roll rotation
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f)); // Animation rotation

        std::vector<float> pos = this->transform.getPosition();
        model = glm::translate(model, glm::vec3(pos[0], pos[1], pos[2]));

        // TODO: Set view and projection of camera to UBO (universal buffer object)?
        Camera* cam = CameraController::getInstance()->getActiveCamera();

        // Pass matrices to shader
        Shader* shader = this->material->getShader();
        unsigned int shaderProgram = shader->getProgram();

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        HandleError::printErrorIf("GL Error setting model matrix on object");

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getView()));
        HandleError::printErrorIf("GL Error setting view matrix on object");

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(cam->getProjection()));
        HandleError::printErrorIf("GL Error setting projection matrix on object");

        glBindVertexArray(this->attributeHandle);
        HandleError::printErrorIf("GL Error binding vertex array on object");

        glDrawElements(GL_TRIANGLES, this->elementSize, GL_UNSIGNED_INT, 0);
        HandleError::printErrorIf("GL Error after draw");
    }
}

void Object::setPointLightingInShader()
{
    // Set lighting uniforms
    std::vector<glm::vec3> lightPositions;
    for (PointLight* light : this->affectingPointLights)
    {
        std::vector<float> pos = light->getPosition();
        lightPositions.push_back(glm::vec3(pos[0], pos[1], pos[2]));
    }

    std::vector<glm::vec3> lightColors;
    for (PointLight* light : this->affectingPointLights)
        lightColors.push_back(glm::vec3(light->getRed(), light->getGreen(), light->getBlue()));

    std::vector<float> lightAmbStrengths;
    std::vector<float> lightSpecStrengths;
    for (PointLight* light : this->affectingPointLights)
    {
        lightAmbStrengths.push_back(light->getAmbientStrength());
    }
    
    // TODO: Set default values for gl variables if point lights not present
    Shader* shader = this->material->getShader();
    unsigned int shaderProgram = shader->getProgram();
    std::vector<float> camPosition = CameraController::getInstance()->getActiveCamera()->getPosition();
    glm::vec3 viewPos(camPosition[0], camPosition[1], camPosition[2]);

    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPositions"), this->affectingPointLights.size(), &lightPositions[0][0]);
    HandleError::printErrorIf("GL Error setting lightPositions on object");

    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColors"), this->affectingPointLights.size(), &lightColors[0][0]);
    HandleError::printErrorIf("GL Error setting lightColors on object");

    glUniform1i(glGetUniformLocation(shaderProgram, "numPointLights"), (int)this->affectingPointLights.size());
    HandleError::printErrorIf("GL Error setting numPointLights on object");

    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    HandleError::printErrorIf("GL Error setting viewPos on object");

    glUniform1fv(glGetUniformLocation(shaderProgram, "plAmbientStrengths"), this->affectingPointLights.size(), &lightAmbStrengths[0]);
    HandleError::printErrorIf("GL Error setting plAmbientStrengths on object");

    glUniform1i(glGetUniformLocation(shaderProgram, "maxShine"), 32);
    HandleError::printErrorIf("GL Error setting maxShine on object");
}

void Object::setDirectionalLightingInShader()
{
    // Gather information from directional lights
    DirectionalLightingController* controller = DirectionalLightingController::getInstance();
    const std::vector<DirectionalLight*>& dirLights = controller->getLights();
    std::vector<glm::vec3> lightDirections;
    std::vector<glm::vec3> lightColors;
    std::vector<float> ambStrengths;
    for (DirectionalLight* dirLight : dirLights)
    {
        lightDirections.push_back(dirLight->getDirection());
        lightColors.push_back(dirLight->getColor());
        ambStrengths.push_back(dirLight->getAmbientStrength());
    }

    // Set gl variables for directional lights - if no lights present, leave gl variables undefined
    // (except for 'numDirLights') because they won't be used

    Shader* shader = this->material->getShader();
    unsigned int shaderProgram = shader->getProgram();
    glUniform1i(glGetUniformLocation(shaderProgram, "numDirLights"), dirLights.size());
    HandleError::printErrorIf("GL Error setting numDirLights on object");

    if (dirLights.size() > 0)
    {
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightColors"), dirLights.size(), &lightColors[0][0]);
        HandleError::printErrorIf("GL Error setting dirLightColors on object");
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDirs"), dirLights.size(), &lightDirections[0][0]);
        HandleError::printErrorIf("GL Error setting lightDirs on object");
        glUniform1fv(glGetUniformLocation(shaderProgram, "dirLightAmbientStrengths"), dirLights.size(), &ambStrengths[0]);
        HandleError::printErrorIf("GL Error setting dirLightAmbientStrengths on object");
    }
}
