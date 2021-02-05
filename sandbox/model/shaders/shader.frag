#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 kd;
uniform vec3 ks;
uniform vec3 ka;
uniform float shininess;

uniform sampler2D diffuseMaps[100];
uniform sampler2D specularMaps[100];

void main() {
  fragColor = vec4(texture(diffuseMaps[0], texCoords).xyz + kd, 1.0);
}