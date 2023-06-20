struct Material {
  vec3 kd;
  vec3 ks;
  vec3 ka;
  vec3 ke;
  float shininess;
  bool hasHeightMap;
  bool hasNormalMap;
  bool hasDisplacementMap;
  bool hasLightMap;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D ambientMap;
layout(binding = 3) uniform sampler2D emissiveMap;
layout(binding = 4) uniform sampler2D heightMap;
layout(binding = 5) uniform sampler2D normalMap;
layout(binding = 6) uniform sampler2D shininessMap;
layout(binding = 7) uniform sampler2D displacementMap;
layout(binding = 8) uniform sampler2D lightMap;

struct PointLight {
  vec3 ke;
  vec3 position;
  float radius;
};

struct DirectionalLight {
  vec3 ke;
  vec3 direction;
};

uniform Material material;

uniform PointLight pointLight;
uniform DirectionalLight directionalLight;