#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D depthMap;

const float zNear = 0.1;
const float zFar = 10000.0;

float linearizeDepth(in float z) {
  float ndc = z * 2.0 - 1.0;
  return (2.0 * zNear * zFar) / (zFar + zNear - ndc * (zFar - zNear));
}

void main() {
  float z = linearizeDepth(texture(depthMap, texCoords).x) / zFar;
  fragColor = vec4(vec3(z), 1.0);
}