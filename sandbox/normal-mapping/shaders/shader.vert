#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;

out vec3 position;
out vec3 normal;
out vec2 texCoords;
out mat3 TBN;

uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);

  // compute T, B, N
  vec3 T = vTangent;
  vec3 N = vNormal;
  // gram-schmidt orthogonalization
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);

  // output to fragment shader
  position = vPosition;
  normal = vNormal;
  texCoords = vTexCoords;
  TBN = mat3(T, B, N);
}