#version 460 core
#include ../../common/shaders/uniforms.glsl

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 tangent;
  vec3 dndu;
  vec3 dndv;
} fs_in;

out vec4 fragColor;

uniform int layerType;

void main() {
  vec3 color = vec3(0);

  if(layerType == 0) {
    color = fs_in.position;
  }
  else if(layerType == 1) {
    color = 0.5 * fs_in.normal + 0.5;
  }
  else if(layerType == 2) {
    color = vec3(fs_in.texCoords, 0.0);
  }
  else if(layerType == 3) {
    color = 0.5 * fs_in.tangent + 0.5;
  }
  else if(layerType == 4) {
    color = 0.5 * fs_in.dndu + 0.5;
  }
  else if(layerType == 5) {
    color = 0.5 * fs_in.dndv + 0.5;
  }
  else if(layerType == 6) {
    color = texture(material.diffuseMap, fs_in.texCoords).xyz + material.kd;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 7) {
    color = texture(material.specularMap, fs_in.texCoords).xyz + material.ks;
  }
  else if(layerType == 8) {
    color = texture(material.ambientMap, fs_in.texCoords).xyz + material.ka;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 9) {
    color = texture(material.emissiveMap, fs_in.texCoords).xyz + material.ke;
    // gamma correction
    color = pow(color, vec3(1.0 / 2.2));
  }
  else if(layerType == 10) {
    color = texture(material.heightMap, fs_in.texCoords).xyz;
  }
  else if(layerType == 11) {
    color = texture(material.normalMap, fs_in.texCoords).xyz;
  }
  else if(layerType == 12) {
    color = texture(material.shininessMap, fs_in.texCoords).xyz;
  }
  else if(layerType == 13) {
    color = texture(material.displacementMap, fs_in.texCoords).xyz;
  }
  else if(layerType == 14) {
    color = texture(material.lightMap, fs_in.texCoords).xyz;
  }

  fragColor = vec4(color, 1.0);
}