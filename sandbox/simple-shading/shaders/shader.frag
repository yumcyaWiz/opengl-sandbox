#version 460 core
#include ../../common/shaders/uniforms.glsl

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 camPos;

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

void main() {
  // view direction
  vec3 viewDir = normalize(camPos - position);

  vec3 kd = texture(material.diffuseMap, texCoords).xyz + material.kd;
  vec3 ks = texture(material.specularMap, texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // directional light
  color += blinnPhong(viewDir, normal, directionalLight.direction, kd, ks, material.shininess) * directionalLight.ke;

  // point lights
  vec3 lightDir = normalize(pointLight.position - position);
  float dist = max(distance(pointLight.position, position) - pointLight.radius, 0.0);
  color += blinnPhong(viewDir, normal, lightDir, kd, ks, material.shininess) * pointLight.ke / pow(dist, 2.0);

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}