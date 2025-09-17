#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

class Shader;

class BlinnPhongMaterial
{
public:
    BlinnPhongMaterial();
    BlinnPhongMaterial(const std::vector<std::string>& texPaths);
    ~BlinnPhongMaterial();

    /**
     * @brief Sets the shader program for this material, if required. Compiles the shader
     *        if its not already compiled.  Binds textures to shader program.
     */
    void prepareRender();

    Shader* getShader() { return this->shader; }
    void setShader(Shader* shader) { this->shader = shader; }

private:
    Shader* shader; // Doesn't own this, MaterialController does
    std::vector<unsigned int> textureHandles;

    bool loadTexture(const std::string& texPath);
    void setMaterialUniformVariables();
    void setShaderProgram(unsigned int shaderProgramHandle);
};

#endif // MATERIAL_H