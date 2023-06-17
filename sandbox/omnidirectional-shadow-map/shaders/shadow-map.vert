#version 460 core
layout (location = 0) in vec3 vPos;

out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
  gl_Position = vec4(vPos, 1.0);
}