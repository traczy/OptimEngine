#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>
#include <vector>

class PointLight;

class Object
{
public:
    Object();
    Object(float* vcData, unsigned int* elementData, float* tangentData, size_t vcSize, size_t eSize, size_t tangentSize);

    ~Object();

    // TODO: Implement
    void setVertexData(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize);

    void addAffectingLight(PointLight* light);

    void render();

    bool loadTexture(const char* path);
    bool compileShader();
    bool buildGeometry();

private:
    float* vData;
    unsigned int* elementBufferData;
    float* tangentData;
    size_t dataSize;
    size_t elementSize;
    size_t tangentSize;

    unsigned int attributeHandle;
    unsigned int vDataHandle;
    unsigned int elementHandle;
    unsigned int tangentHandle;

    unsigned int shaderProgramHandle;
    std::vector<unsigned int> textureHandles;

    std::vector<PointLight*> affectingLights;

    void bindTexturesForRender();
    void setLightingInShader();
};

#endif // OBJECT_H