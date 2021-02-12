#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
} vs_out;

uniform mat4 viewProjection;

void main() {
  gl_Position = viewProjection * vec4(vPosition, 1.0);
  vs_out.position = vPosition;
  vs_out.normal = vNormal;
  vs_out.texCoords = vTexCoords;
}