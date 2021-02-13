#version 330 core
#include ../../common/shaders/uniforms.glsl

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 viewDirection;
  vec3 pointLightDirection;
  vec3 directionalLightDirection;
  mat3 TBN;
} fs_in;

out vec4 fragColor;

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
  vec3 n = vec3(0, 0, 1);
  if(useNormalMap) {
    n = normalize(2.0 * texture(material.normalMap, fs_in.texCoords).xyz - 1.0);
  }

  vec3 kd = texture(material.diffuseMap, fs_in.texCoords).xyz + material.kd;
  vec3 ks = texture(material.specularMap, fs_in.texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // point light
  float dist = max(distance(pointLight.position, fs_in.position) - pointLight.radius, 0.0);
  color += blinnPhong(fs_in.viewDirection, n, fs_in.pointLightDirection, kd, ks, material.shininess) * pointLight.ke / pow(dist, 2.0);

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  if(showNormal) {
    fragColor = vec4(0.5 * fs_in.TBN * n + 0.5, 1.0);
  }
  else {
    fragColor = vec4(color, 1.0);
  }
}