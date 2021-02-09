#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;

out VS_OUT {
  vec4 tangent;
  vec4 binormal;
  vec4 normal;
} vs_out;

void main() {
  gl_Position = vec4(vPosition, 1.0);
  vs_out.tangent = vec4(vTangent, 0.0);
  vs_out.normal = vec4(vNormal, 0.0);
  vs_out.binormal = vec4(cross(vNormal, vTangent), 0.0);
}