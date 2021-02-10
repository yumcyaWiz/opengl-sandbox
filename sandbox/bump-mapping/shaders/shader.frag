#version 330 core
#include ../../common/shaders/uniforms.glsl

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 tangent;
  vec3 binormal;
  vec3 dndu;
  vec3 dndv;
} fs_in;

out vec4 fragColor;

uniform vec3 camPos;
uniform bool useHeightMap;
uniform float heightMapScale;

// Blinn-Phong reflection model
vec3 blinnPhong(in vec3 viewDir, in vec3 normal, in vec3 lightDir, in vec3 kd, in vec3 ks, in float shininess) {
  vec3 diffuse = max(dot(lightDir, normal), 0.0) * kd;

  vec3 h = normalize(lightDir + viewDir); // half-vector
  vec3 specular = pow(max(dot(h, normal), 0.0), shininess) * ks;

  return diffuse + specular;
}

vec3 computeNormalFromHeightMap(in sampler2D heightMap, in vec2 texCoords, in vec3 n, in vec3 dpdu, in vec3 dpdv, in vec3 dndu, in vec3 dndv) {
  // compute differential of height map
  vec2 texelSize = 1.0 / textureSize(heightMap, 0);
  float dhdu = (heightMapScale * textureOffset(heightMap, texCoords, ivec2(1, 0)).x - heightMapScale * textureOffset(heightMap, texCoords, ivec2(-1, 0)).x) / (2.0 * texelSize.x);
  float dhdv = (heightMapScale * textureOffset(heightMap, texCoords, ivec2(0, 1)).x - heightMapScale * textureOffset(heightMap, texCoords, ivec2(0, -1)).x) / (2.0 * texelSize.y);

  // computed modified tangent, binormal vector
  float h = heightMapScale * texture(heightMap, texCoords).x;
  vec3 dpdu_mod = dpdu + dhdu * n + h * dndu;
  vec3 dpdv_mod = dpdv + dhdv * n + h * dndv;

  return normalize(cross(dpdu_mod, dpdv_mod));
}

void main() {
  // view direction
  vec3 viewDir = normalize(camPos - fs_in.position);

  vec3 normal = fs_in.normal;
  if(useHeightMap) {
    normal = computeNormalFromHeightMap(material.heightMap, fs_in.texCoords, fs_in.normal, fs_in.tangent, fs_in.binormal, fs_in.dndu, fs_in.dndv);
  }

  vec3 kd = texture(material.diffuseMap, fs_in.texCoords).xyz + material.kd;
  vec3 ks = texture(material.specularMap, fs_in.texCoords).xyz + material.ks;

  vec3 color = vec3(0);

  // directional light
  color += blinnPhong(viewDir, normal, directionalLight.direction, kd, ks, material.shininess) * directionalLight.ke;

  // point lights
  for(int i = 0; i < n_PointLights; ++i) {
    vec3 lightDir = normalize(pointLights[i].position - fs_in.position);
    float dist = max(distance(pointLights[i].position, fs_in.position) - pointLights[i].radius, 0.0);
    color += blinnPhong(viewDir, normal, lightDir, kd, ks, material.shininess) * pointLights[i].ke / pow(dist, 2.0);
  }

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}