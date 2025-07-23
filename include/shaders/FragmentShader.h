const char* fragmentShader = R"(
#version 330 core

#define MAX_LIGHTS 16

in vec2 TexCoord;
in mat3 TBN;
in vec3 FragPos;
out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D normalMap; // Normal map
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform int numLights; // active number of lights
uniform vec3 viewPos; // Camera position
void main() {
   // Sample normal from normal map (tangent space)
   vec3 normal = texture(normalMap, TexCoord).rgb;
   normal = normalize(normal * 2.0 - 1.0); // Convert from [0,1] to [-1,1]
   normal = normalize(TBN * normal); // Transform to world space

   // Initialize lighting components
   vec3 ambient = vec3(0.0);
   vec3 diffuse = vec3(0.0);
   vec3 specular = vec3(0.0);
   float ambientStrength = 0.1;
   float specularStrength = 0.5;

   // Compute contribution from each light
   for (int i = 0; i < numLights; ++i) {
      // Ambient
      ambient += ambientStrength * lightColors[i];

      // Diffuse
      vec3 lightDir = normalize(lightPositions[i] - FragPos);
      float diff = max(dot(normal, lightDir), 0.0);
      diffuse += diff * lightColors[i];

      // Specular (Blinn-Phong)
      vec3 viewDir = normalize(viewPos - FragPos);
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
      specular += specularStrength * spec * lightColors[i];
   }

   // Combine lighting with texture
   vec3 result = (ambient + diffuse + specular) * texture(texture1, TexCoord).rgb;
   FragColor = vec4(result, 1.0);
}
)";