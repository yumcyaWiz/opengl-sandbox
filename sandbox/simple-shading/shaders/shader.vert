#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 position;
out vec3 normal;
out vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);
  position = vPosition;
  normal = vNormal;
  texCoords = vTexCoords;
}