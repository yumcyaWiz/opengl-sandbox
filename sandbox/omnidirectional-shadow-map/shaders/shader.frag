#version 460 core
#include ../../common/shaders/uniforms.glsl

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
} fs_in;

out vec4 fragColor;

uniform vec3 camPos;
uniform float shadowBias;
uniform samplerCube shadowMap;
uniform float zFar;

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

float shadowMapping(in vec3 position) {
  vec3 lightToPos = position - pointLight.position;
  float currentDepth = length(lightToPos);
  float closestDepth = zFar * texture(shadowMap, lightToPos).x;
  return currentDepth > closestDepth + shadowBias ? 1.0 : 0.0;
}

void main() {
  // view direction
  vec3 viewDir = normalize(camPos - fs_in.position);

  vec3 kd = texture(diffuseMap, fs_in.texCoords).xyz + material.kd;
  vec3 ks = texture(specularMap, fs_in.texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // shadow mapping
  float shadow = shadowMapping(fs_in.position);

  // sahding
  vec3 lightDir = normalize(pointLight.position - fs_in.position);
  float dist = max(distance(pointLight.position, fs_in.position) - pointLight.radius, 0.0);
  color += (1.0 - shadow) * blinnPhong(viewDir, fs_in.normal, lightDir, kd, ks, material.shininess) * pointLight.ke / pow(dist, 2.0);

  // ambient
  color += 0.01 * shadow * kd;

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}