#version 330 core
#include ../../common/shaders/uniforms.glsl

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform int layerType;

void main() {
  vec3 color = vec3(0);

  if(layerType == 0) {
    color = position;
  }
  else if(layerType == 1) {
    color = 0.5 * normal + 0.5;
  }
  else if(layerType == 2) {
    color = vec3(texCoords, 0.0);
  }
  else if(layerType == 3) {
    color = texture(material.diffuseMap, texCoords).xyz + material.kd;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 4) {
    color = texture(material.specularMap, texCoords).xyz + material.ks;
  }
  else if(layerType == 5) {
    color = texture(material.ambientMap, texCoords).xyz + material.ka;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 6) {
    color = texture(material.emissiveMap, texCoords).xyz + material.ke;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 7) {
    color = texture(material.heightMap, texCoords).xyz;
  }
  else if(layerType == 8) {
    color = texture(material.normalMap, texCoords).xyz;
  }
  else if(layerType == 9) {
    color = texture(material.shininessMap, texCoords).xyz;
  }
  else if(layerType == 10) {
    color = texture(material.displacementMap, texCoords).xyz;
  }
  else if(layerType == 11) {
    color = texture(material.lightMap, texCoords).xyz;
  }

  fragColor = vec4(color, 1.0);
}