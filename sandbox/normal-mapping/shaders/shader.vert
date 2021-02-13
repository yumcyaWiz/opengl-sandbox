#version 330 core
#include ../../common/shaders/uniforms.glsl
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 viewDirection;
  vec3 pointLightDirection;
  vec3 directionalLightDirection;
  mat3 TBN;
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);

  // compute TBN matrix
  vec3 T = vTangent;
  vec3 N = vNormal;
  // gram-schmidt orthogonalization
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  mat3 TBN = mat3(T, B, N);
  mat3 TBN_inv = transpose(TBN);

  // output to fragment shader
  vs_out.position = vPosition;
  vs_out.normal = vNormal;
  vs_out.texCoords = vTexCoords;
  vs_out.viewDirection = normalize(TBN_inv * (camPos - vPosition));
  vs_out.pointLightDirection = normalize(TBN_inv * (pointLight.position - vPosition));
  vs_out.directionalLightDirection = normalize(TBN_inv * directionalLight.direction);
  vs_out.TBN = TBN; // for showing normal in world space
}