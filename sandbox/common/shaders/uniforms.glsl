uniform vec3 kd;
uniform vec3 ks;
uniform vec3 ka;
uniform float shininess;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

struct PointLight {
  vec3 ke;
  vec3 position;
  float radius;
};

struct DirectionalLight {
  vec3 ke;
  vec3 direction;
};

uniform int n_PointLights;
uniform PointLight pointLights[100];
uniform DirectionalLight directionalLight;