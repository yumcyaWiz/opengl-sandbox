#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vDndu;
layout (location = 5) in vec3 vDndv;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 tangent;
  vec3 dndu;
  vec3 dndv;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);
  vs_out.position = vPosition;
  vs_out.normal = vNormal;
  vs_out.texCoords = vTexCoords;
  vs_out.tangent = vTangent;
  vs_out.dndu = vDndu;
  vs_out.dndv = vDndv;
}