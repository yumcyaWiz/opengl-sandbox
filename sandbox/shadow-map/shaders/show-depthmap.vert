#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 texCoords;

void main() {
  gl_Position = vec4(vPosition, 1.0);
  texCoords = vTexCoords;
}