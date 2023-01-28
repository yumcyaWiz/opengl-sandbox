struct Material {
  vec3 kd;
  vec3 ks;
  vec3 ka;
  vec3 ke;
  float shininess;
  sampler2D diffuseMap;
  sampler2D specularMap;
  sampler2D ambientMap;
  sampler2D emissiveMap;
  bool hasHeightMap;
  sampler2D heightMap;
  bool hasNormalMap;
  sampler2D normalMap;
  sampler2D shininessMap;
  bool hasDisplacementMap;
  sampler2D displacementMap;
  bool hasLightMap;
  sampler2D lightMap;
};

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