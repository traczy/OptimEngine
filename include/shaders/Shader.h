#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader();
    ~Shader();

    void setVertexShaderSource(const char* src) { this->vertexSource = src; }
    void setFragmentShaderSource(const char* src) { this->fragmentSource = src; }

    bool compile();

    unsigned int getProgram() { return this->programHandle; }

private:
    const char* vertexSource;
    const char* fragmentSource;

    unsigned int programHandle;
};

#endif // SHADER_H