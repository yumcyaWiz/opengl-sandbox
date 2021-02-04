#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 kd;
uniform vec3 ks;
uniform vec3 ka;
uniform float shininess;

uniform sampler2D diffuseTextures[100];
uniform sampler2D specularTextures[100];

void main() {
  // fragColor = vec4(kd + texture(diffuseTextures[0], texCoords).xyz, 1.0);
  fragColor = vec4(normal, 1.0);
}