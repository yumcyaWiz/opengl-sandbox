#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

in VS_OUT {
  vec4 tangent;
  vec4 binormal;
  vec4 normal;
} gs_in[];

out GS_OUT {
  vec3 color;
} gs_out;

uniform mat4 view;
uniform mat4 projection;
uniform float lineLength;

void visualizeTangentSpace(in mat4 viewProjection, int index) {
  // tangent
  gs_out.color = vec3(1, 0, 0);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + lineLength * gs_in[index].tangent);
  EmitVertex();
  EndPrimitive();

  // binormal
  gs_out.color = vec3(0, 1, 0);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + lineLength * gs_in[index].binormal);
  EmitVertex();
  EndPrimitive();

  // normal
  gs_out.color = vec3(0, 0, 1);
  gl_Position = viewProjection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = viewProjection * (gl_in[index].gl_Position + lineLength * gs_in[index].normal);
  EmitVertex();
  EndPrimitive();
}

void main() {
  mat4 viewProjection = projection * view;

  visualizeTangentSpace(viewProjection, 0);
  visualizeTangentSpace(viewProjection, 1);
  visualizeTangentSpace(viewProjection, 2);
}