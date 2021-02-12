#version 330 core

in GS_OUT {
  vec4 position;
} fs_in;

uniform vec3 lightPosition;
uniform float zFar;

void main() {
  // compute depth from lightPos
  float depth = length(fs_in.position.xyz - lightPosition);

  // normalize to [0, 1]
  depth = depth / zFar;

  gl_FragDepth = depth;
}