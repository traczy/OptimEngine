#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>

class Object
{
public:
    Object();
    Object(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize);

    ~Object();

    // TODO: Implement
    void setVertexData(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize);

    void render();

    bool loadTexture(const char* path);
    bool compileShader();
    bool buildGeometry();

private:
    float* vertexAndColorData;
    unsigned int* elementBufferData;
    size_t dataSize;
    size_t elementSize;

    unsigned int attributeHandle;
    unsigned int vcDataHandle;
    unsigned int elementHandle;

    unsigned int shaderProgramHandle;
    unsigned int textureHandle;
};

#endif // OBJECT_H