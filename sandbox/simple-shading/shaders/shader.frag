#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 kd;
uniform vec3 ks;
uniform vec3 ka;
uniform float shininess;

uniform sampler2D diffuseMaps[100];
uniform sampler2D specularMaps[100];

struct PointLight {
  vec3 ke;
  vec3 position;
  float radius;
};

struct DirectionalLight {
  vec3 ke;
  vec3 direction;
};

uniform PointLight pointLights[100];
uniform DirectionalLight directionalLight;

uniform vec3 lightDir;
uniform vec3 camPos;

void main() {
  // diffuse
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * (texture(diffuseMaps[0], texCoords).xyz + kd);

  // specular
  vec3 v = normalize(camPos - position); // view direction
  vec3 h = normalize(lightDir + v); // half vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * (texture(specularMaps[0], texCoords).xyz + ks);

  vec3 color = diffuse + specular;

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}