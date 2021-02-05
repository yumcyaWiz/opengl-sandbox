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
uniform bool hasDiffuseTextures;
uniform bool hasSpecularTextures;

void main() {
  if(hasDiffuseTextures) {
    fragColor = texture(diffuseTextures[0], texCoords);
  }
  else {
    fragColor = vec4(kd, 1.0);
  }
}