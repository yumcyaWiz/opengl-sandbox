#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

in VS_OUT {
  vec3 tangent;
  vec3 binormal;
  vec3 normal;
} gs_in[];

out gl_PerVertex
{
  vec4 gl_Position;
};
out GS_OUT {
  vec3 color;
} gs_out;

uniform mat4 view;
uniform mat4 projection;
uniform float lineLength;

void visualizeTangentSpace(in mat4 viewProjection, in int index) {
  // tangent
  gs_out.color = vec3(1, 0, 0);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + vec4(lineLength * gs_in[index].tangent, 0.0));
  EmitVertex();
  EndPrimitive();

  // binormal
  gs_out.color = vec3(0, 1, 0);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + vec4(lineLength * gs_in[index].binormal, 0.0));
  EmitVertex();
  EndPrimitive();

  // normal
  gs_out.color = vec3(0, 0, 1);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + vec4(lineLength * gs_in[index].normal, 0.0));
  EmitVertex();
  EndPrimitive();
}

void main() {
  mat4 viewProjection = projection * view;

  visualizeTangentSpace(viewProjection, 0);
  visualizeTangentSpace(viewProjection, 1);
  visualizeTangentSpace(viewProjection, 2);
}