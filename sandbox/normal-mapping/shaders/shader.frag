#version 460 core
#include ../../common/shaders/uniforms.glsl

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  mat3 TBN;
} fs_in;

out vec4 fragColor;

uniform vec3 camPos;
uniform bool useNormalMap;
uniform bool showNormal;

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

void main() {
  // compute normal
  vec3 n = fs_in.normal;
  if(useNormalMap) {
    n = normalize(fs_in.TBN * (2.0 * texture(normalMap, fs_in.texCoords).xyz - 1.0));
  }

  // view direction
  vec3 viewDir = normalize(camPos - fs_in.position);

  vec3 kd = texture(diffuseMap, fs_in.texCoords).xyz + material.kd;
  vec3 ks = texture(specularMap, fs_in.texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // directional light
  color += blinnPhong(viewDir, n, directionalLight.direction, kd, ks, material.shininess) * directionalLight.ke;

  // point lights
  vec3 lightDir = normalize(pointLight.position - fs_in.position);
  float dist = max(distance(pointLight.position, fs_in.position) - pointLight.radius, 0.0);
  color += blinnPhong(viewDir, n, lightDir, kd, ks, material.shininess) * pointLight.ke / pow(dist, 2.0);

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  if(showNormal) {
    fragColor = vec4(0.5 * n + 0.5, 1.0);
  }
  else {
    fragColor = vec4(color, 1.0);
  }
}