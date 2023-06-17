#version 460 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out gl_PerVertex {
  vec4 gl_Position;
};
out vec3 color;

void main() {
  gl_Position = vec4(vPosition, 1.0);
  color = vColor;
}