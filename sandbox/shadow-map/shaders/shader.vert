#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 position;
out vec3 normal;
out vec2 texCoords;
out vec4 positionLightSpace;

uniform mat4 viewProjection;
uniform mat4 lightSpaceMatrix;

void main() {
  gl_Position = viewProjection * vec4(vPosition, 1.0);
  position = vPosition;
  normal = vNormal;
  texCoords = vTexCoords;
  positionLightSpace = lightSpaceMatrix * vec4(vPosition, 1.0);
}