#include "mesh.hpp"

namespace ogls
{

Mesh::Mesh() {}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices, MaterialID material_id)
    : vertices{vertices}, indices{indices}, material_id{material_id}
{
  // TODO: maybe this is bad, because we are sending all the model data to the
  // GPU. This is consuming a lot of VRAM.
  vertex_buffer.setData(vertices, GL_STATIC_DRAW);
  index_buffer.setData(indices, GL_STATIC_DRAW);

  vao.bindVertexBuffer(vertex_buffer, 0, 0, sizeof(Vertex));
  vao.bindElementBuffer(index_buffer);

  // position
  vao.activateVertexAttribution(0, 0, 3, GL_FLOAT, 0);
  // normal
  vao.activateVertexAttribution(0, 1, 3, GL_FLOAT, offsetof(Vertex, normal));
  // texcoords
  vao.activateVertexAttribution(0, 2, 2, GL_FLOAT, offsetof(Vertex, texcoords));
  // tangent
  vao.activateVertexAttribution(0, 3, 3, GL_FLOAT, offsetof(Vertex, tangent));
  // dndu
  vao.activateVertexAttribution(0, 4, 3, GL_FLOAT, offsetof(Vertex, dndu));
  // dndv
  vao.activateVertexAttribution(0, 5, 3, GL_FLOAT, offsetof(Vertex, dndv));
}

Mesh::Mesh(Mesh&& other)
{
  vertices = std::move(other.vertices);
  indices = std::move(other.indices);
  material_id = std::move(other.material_id);
  vertex_buffer = std::move(other.vertex_buffer);
  index_buffer = std::move(other.index_buffer);
  vao = std::move(other.vao);
}

Mesh& Mesh::operator=(Mesh&& other)
{
  if (this == &other) return *this;
  vertices = std::move(other.vertices);
  indices = std::move(other.indices);
  material_id = std::move(other.material_id);
  vertex_buffer = std::move(other.vertex_buffer);
  index_buffer = std::move(other.index_buffer);
  vao = std::move(other.vao);
  return *this;
}

void Mesh::draw(const Pipeline& pipeline, const Material& material,
                const std::vector<Texture>& textures) const
{
  // set texture uniform
  if (material.diffuse_map) {
    const Texture& tex = textures[material.diffuse_map.value()];
    tex.bindToTextureUnit(0);
  }

  if (material.specular_map) {
    const Texture& tex = textures[material.specular_map.value()];
    tex.bindToTextureUnit(1);
  }

  if (material.ambient_map) {
    const Texture& tex = textures[material.ambient_map.value()];
    tex.bindToTextureUnit(2);
  }

  if (material.emissive_map) {
    const Texture& tex = textures[material.emissive_map.value()];
    tex.bindToTextureUnit(3);
  }

  if (material.height_map) {
    const Texture& tex = textures[material.height_map.value()];
    tex.bindToTextureUnit(4);

    // TODO: remove this
    pipeline.setUniform("material.hasHeightMap", true);
  } else {
    pipeline.setUniform("material.hasHeightMap", false);
  }

  if (material.normal_map) {
    const Texture& tex = textures[material.normal_map.value()];
    tex.bindToTextureUnit(5);

    // TODO: remove this
    pipeline.setUniform("material.hasNormalMap", true);
  } else {
    pipeline.setUniform("material.hasNormalMap", false);
  }

  if (material.shininess_map) {
    const Texture& tex = textures[material.shininess_map.value()];
    tex.bindToTextureUnit(6);
  }

  if (material.displacement_map) {
    const Texture& tex = textures[material.displacement_map.value()];
    tex.bindToTextureUnit(7);

    // TODO: remove this
    pipeline.setUniform("material.hasDisplacementMap", true);
  } else {
    pipeline.setUniform("material.hasDisplacementMap", false);
  }

  if (material.light_map) {
    const Texture& tex = textures[material.light_map.value()];
    tex.bindToTextureUnit(8);

    // TODO: remove this
    pipeline.setUniform("material.hasLightMap", true);
  } else {
    pipeline.setUniform("material.hasLightMap", false);
  }

  // set material
  if (material.diffuse_map) {
    pipeline.setUniform("material.kd", glm::vec3(0));
  } else {
    pipeline.setUniform("material.kd", material.kd);
  }

  if (material.specular_map) {
    pipeline.setUniform("material.ks", glm::vec3(0));
  } else {
    pipeline.setUniform("material.ks", material.ks);
  }

  if (material.ambient_map) {
    pipeline.setUniform("material.ka", glm::vec3(0));
  } else {
    pipeline.setUniform("material.ka", material.ka);
  }

  if (material.emissive_map) {
    pipeline.setUniform("material.ke", glm::vec3(0));
  } else {
    pipeline.setUniform("material.ke", material.ke);
  }

  pipeline.setUniform("material.shininess", material.shininess);

  // draw mesh
  pipeline.activate();
  vao.activate();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.deactivate();
  pipeline.deactivate();

  // reset texture uniforms
  pipeline.setUniform("material.hasHeightMap", false);
  pipeline.setUniform("material.hasNormalMap", false);
  pipeline.setUniform("material.hasDisplacementMap", false);
  pipeline.setUniform("material.hasLightMap", false);
}

uint32_t Mesh::getNumberOfVertices() const { return vertices.size(); }

uint32_t Mesh::getNumberOfFaces() const { return indices.size() / 3; }

uint32_t Mesh::getMaterialID() const { return material_id; }

}  // namespace ogls