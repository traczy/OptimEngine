const char* fragmentShader = R"(
#version 330 core
in vec2 TexCoord;
in mat3 TBN;
in vec3 FragPos;
out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D normalMap; // Normal map
uniform vec3 lightPos; // Point light position
uniform vec3 lightColor; // Light color
uniform vec3 viewPos; // Camera position
void main() {
   // Sample normal from normal map (tangent space)
   vec3 normal = texture(normalMap, TexCoord).rgb;
   normal = normalize(normal * 2.0 - 1.0); // Convert from [0,1] to [-1,1]
   normal = normalize(TBN * normal); // Transform to world space

   // Ambient
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   // Diffuse
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // Specular (Blinn-Phong)
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 halfwayDir = normalize(lightDir + viewDir);
   float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0); // Higher shininess for Blinn-Phong
   vec3 specular = specularStrength * spec * lightColor;

   // Combine lighting with texture
   vec3 result = (ambient + diffuse + specular) * texture(texture1, TexCoord).rgb;
   FragColor = vec4(result, 1.0);
}
)";