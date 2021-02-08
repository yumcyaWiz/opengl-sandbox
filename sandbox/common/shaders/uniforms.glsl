struct Material {
  vec3 kd;
  vec3 ks;
  vec3 ka;
  float shininess;
  sampler2D diffuseMap;
  sampler2D specularMap;
  sampler2D ambientMap;
  sampler2D emissivemap;
  sampler2D heightMap;
  sampler2D normalMap;
  sampler2D shininessMap;
  sampler2D displacementMap;
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

uniform int n_PointLights;
uniform PointLight pointLights[100];
uniform DirectionalLight directionalLight;