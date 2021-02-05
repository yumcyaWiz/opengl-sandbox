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

uniform int n_PointLights;
uniform PointLight pointLights[100];
uniform DirectionalLight directionalLight;

uniform vec3 camPos;

vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

void main() {
  vec3 viewDir = normalize(camPos - position); // view direction

  vec3 diffuse = texture(diffuseMaps[0], texCoords).xyz + kd;
  vec3 specular = texture(specularMaps[0], texCoords).xyz + ks;

  vec3 color = vec3(0);

  // directional light
  color += blinnPhong(viewDir, normal, directionalLight.direction, diffuse, specular, shininess) * directionalLight.ke;

  // point lights
  for(int i = 0; i < n_PointLights; ++i) {
    vec3 lightDir = normalize(pointLights[i].position - position);
    float dist = max(distance(pointLights[i].position, position) - pointLights[i].radius, 0.0);
    color += blinnPhong(viewDir, normal, lightDir, diffuse, specular, shininess) * pointLights[i].ke / pow(dist, 2.0);
  }

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}