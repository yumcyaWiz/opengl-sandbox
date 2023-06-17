#version 460 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out gl_PerVertex {
  vec4 gl_Position;
};

uniform mat4 lightSpaceMatrix;

void main() {
  gl_Position = lightSpaceMatrix * vec4(vPosition, 1.0);
}