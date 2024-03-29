#version 460 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;

out gl_PerVertex {
  vec4 gl_Position;
};
out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  mat3 TBN;
} vs_out;

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
  vs_out.position = vPosition;
  vs_out.normal = vNormal;
  vs_out.texCoords = vTexCoords;
  vs_out.TBN = mat3(T, B, N);
}