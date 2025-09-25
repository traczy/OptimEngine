const char* fragmentShader = R"(
#version 330 core

#define MAX_POINT_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 5

in vec2 TexCoord;
in mat3 TBN;
in vec3 FragPos;
out vec4 FragColor;
uniform sampler2D diffuseMap; // diffuse map
uniform sampler2D normalMap; // Normal map
uniform sampler2D specMap; // specular/reflectance map
uniform sampler2D roughnessMap; // roughness map
uniform vec3 viewPos; // Camera position
uniform int maxShine;

// Directional light gl variables
uniform int numDirLights; // active number of directional lights
uniform vec3 dirLightColors[MAX_DIRECTIONAL_LIGHTS];
uniform vec3 lightDirs[MAX_DIRECTIONAL_LIGHTS];
uniform float dirLightAmbientStrengths[MAX_DIRECTIONAL_LIGHTS];

// Point light gl variables
uniform vec3 lightPositions[MAX_POINT_LIGHTS];
uniform vec3 lightColors[MAX_POINT_LIGHTS];
uniform int numPointLights; // active number of point lights affecting this object
uniform float plAmbientStrengths[MAX_POINT_LIGHTS];

void main() {

   // Sample normal from normal map (tangent space)
   vec3 normal = texture(normalMap, TexCoord).rgb;
   normal = normalize(normal * 2.0 - 1.0); // Convert from [0,1] to [-1,1]
   normal = normalize(TBN * normal); // Transform to world space

   // Sample specular (reflectance) map
   float specStrength = texture(specMap, TexCoord).r; // Assumes grayscale map

   // Obtain 'shininess' exponent for blinn-phong model from roughness map
   float roughnessStrength = texture(roughnessMap, TexCoord).r; // Assumes grayscale map
   float shineExp = (1.0 - roughnessStrength) * maxShine;

   // Initialize lighting components
   vec3 ambient = vec3(0.0);
   vec3 diffuse = vec3(0.0);
   vec3 specular = vec3(0.0);

   // Calculate viewing direction
   vec3 viewDir = normalize(viewPos - FragPos);

   //=========================================
   // Directional Lighting
   //=========================================
   for (int i = 0; i < numDirLights; ++i)
   {
      // Ambient lighting
      ambient += dirLightAmbientStrengths[i] * dirLightColors[i]; // dlAmbientStrengths should come from AO map but can use diffuse texture color

      // Diffuse lighting
      vec3 lightDirNorm = normalize(-lightDirs[i]); // Negative - lightDir points towards the light so we want the opposite of that
      float diff = max(dot(normal, lightDirNorm), 0.0);
      diffuse += diff * dirLightColors[i];

      // Specular lighting (Blinn-Phong)
      vec3 reflectDir = reflect(-lightDirNorm, normal);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineExp); // Exponent should be gotten from roughness map
      specular += specStrength * spec * dirLightColors[i]; // dirLightSpecularStrengths should be gotten from reflectance? map
   }

   //=========================================
   // Point Lighting
   //=========================================
   // Compute contribution from each light
   for (int i = 0; i < numPointLights; ++i) {
      // Ambient
      ambient += plAmbientStrengths[i] * lightColors[i]; // plAmbientStrengths should come from AO map but can use diffuse texture color

      // Diffuse
      vec3 lightDir = normalize(lightPositions[i] - FragPos);
      float diff = max(dot(normal, lightDir), 0.0);
      diffuse += diff * lightColors[i];

      // Specular (Blinn-Phong)
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float spec = pow(max(dot(normal, halfwayDir), 0.0), shineExp); // Exponent should be gotten from roughness map
      specular += specStrength * spec * lightColors[i]; // plSpecularStrengths should be gotten from reflectance? map
   }

   // Combine lighting with texture
   vec3 result = (ambient + diffuse + specular) * texture(diffuseMap, TexCoord).rgb;
   FragColor = vec4(result, 1.0);

}
)";
