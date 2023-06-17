#version 460 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;

out VS_OUT {
  vec3 tangent;
  vec3 binormal;
  vec3 normal;
} vs_out;

void main() {
  gl_Position = vec4(vPosition, 1.0);
  vs_out.tangent = vTangent;
  vs_out.normal = vNormal;
  vs_out.binormal = cross(vNormal, vTangent);
}