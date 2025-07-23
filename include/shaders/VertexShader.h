const char* vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
out vec2 TexCoord;
out mat3 TBN;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0)); // Fragment position in world space

    // Compute TBN matrix
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(cross(T, B)); // Compute normal from tangent and bitangent
    TBN = mat3(T, B, N);
}
)";