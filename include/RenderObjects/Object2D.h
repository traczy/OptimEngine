#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <cstddef>

class Object2D
{
public:
    Object2D();
    Object2D(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize);

    ~Object2D();

    // TODO: Implement
    void setVertexData(float* vcData, unsigned int* elementData, size_t vcSize, size_t eSize);

    void render();

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
};

#endif // OBJECT2D_H