#version 460 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vDndu;
layout (location = 5) in vec3 vDndv;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  vec3 tangent;
  vec3 binormal;
  vec3 dndu;
  vec3 dndv;
  mat3 TBN;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);
  vs_out.position = vPosition;
  vs_out.normal = vNormal;
  vs_out.texCoords = vTexCoords;

  // gram-schmidt orthogonalization
  vs_out.tangent = normalize(vTangent - dot(vTangent, vNormal) * vNormal);
  vs_out.binormal = cross(vNormal, vs_out.tangent);

  vs_out.dndu = vDndu;
  vs_out.dndv = vDndv;
  vs_out.TBN = mat3(vs_out.tangent, vs_out.binormal, vs_out.normal);
}