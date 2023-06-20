#version 460 core
#include ../../common/shaders/uniforms.glsl

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

void main() {
  vec3 color = texture(diffuseMap, texCoords).xyz + material.kd;

  // gamma correction
  color = pow(color, vec3(1.0 / 2.2));

  fragColor = vec4(color, 1.0);
}