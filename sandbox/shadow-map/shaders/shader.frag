#version 330 core
#include ../../common/shaders/uniforms.glsl

in vec3 position;
in vec3 normal;
in vec2 texCoords;
in vec4 positionLightSpace;

out vec4 fragColor;

uniform vec3 camPos;
uniform sampler2D depthMap;

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

float testShadow(in vec4 positionLightSpace) {
  // projection
  vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;

  // convert from NDC to [0, 1]
  projCoords = projCoords * 0.5 + 0.5;

  float closestDepth = texture(depthMap, projCoords.xy).x;
  float currentDepth = projCoords.z;

  return currentDepth > closestDepth ? 1.0 : 0.0;
}

void main() {
  // view direction
  vec3 viewDir = normalize(camPos - position);

  vec3 kd = texture(material.diffuseMap, texCoords).xyz + material.kd;
  vec3 ks = texture(material.specularMap, texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // shadow test
  float shadow = testShadow(positionLightSpace);

  // directional light
  color += (1.0 - shadow) * blinnPhong(viewDir, normal, directionalLight.direction, kd, ks, material.shininess) * directionalLight.ke;

  // point lights
  for(int i = 0; i < n_PointLights; ++i) {
    vec3 lightDir = normalize(pointLights[i].position - position);
    float dist = max(distance(pointLights[i].position, position) - pointLights[i].radius, 0.0);
    color += (1.0 - shadow) * blinnPhong(viewDir, normal, lightDir, kd, ks, material.shininess) * pointLights[i].ke / pow(dist, 2.0);
  }

  // ambient
  color += 0.01 * shadow * kd;

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}