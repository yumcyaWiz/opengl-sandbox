#include "mesh.hpp"

namespace ogls
{

Mesh::Mesh() {}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices, const Material& material)
    : vertices{vertices}, indices{indices}, material{material}
{
  vbo.setData(vertices, GL_STATIC_DRAW);
  ebo.setData(indices, GL_STATIC_DRAW);

  vao.bindVertexBuffer(vbo, 0, 0, sizeof(Vertex));
  vao.bindElementBuffer(ebo);

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
  material = std::move(other.material);
  vbo = std::move(other.vbo);
  ebo = std::move(other.ebo);
  vao = std::move(other.vao);
}

Mesh& Mesh::operator=(Mesh&& other)
{
  if (this == &other) return *this;
  vertices = std::move(other.vertices);
  indices = std::move(other.indices);
  material = std::move(other.material);
  vbo = std::move(other.vbo);
  ebo = std::move(other.ebo);
  vao = std::move(other.vao);
  return *this;
}

void Mesh::draw(const Pipeline& pipeline, const Shader& shader,
                const std::vector<Texture>& textures) const
{
  // set texture uniform
  if (material.diffuse_map) {
    const Texture& tex = textures[material.diffuse_map.value()];
    tex.bindToTextureUnit(1);
    shader.setUniform("material.diffuseMap", 1);
  }

  if (material.specular_map) {
    const Texture& tex = textures[material.specular_map.value()];
    tex.bindToTextureUnit(2);
    shader.setUniform("material.specularMap", 2);
  }

  if (material.ambient_map) {
    const Texture& tex = textures[material.ambient_map.value()];
    tex.bindToTextureUnit(3);
    shader.setUniform("material.ambientMap", 3);
  }

  if (material.emissive_map) {
    const Texture& tex = textures[material.emissive_map.value()];
    tex.bindToTextureUnit(4);
    shader.setUniform("material.emissiveMap", 4);
  }

  if (material.height_map) {
    const Texture& tex = textures[material.height_map.value()];
    tex.bindToTextureUnit(5);
    shader.setUniform("material.heightMap", 5);

    // TODO: remove this
    shader.setUniform("material.hasHeightMap", true);
  } else {
    shader.setUniform("material.hasHeightMap", false);
  }

  if (material.normal_map) {
    const Texture& tex = textures[material.normal_map.value()];
    tex.bindToTextureUnit(6);
    shader.setUniform("material.normalMap", 6);

    // TODO: remove this
    shader.setUniform("material.hasNormalMap", true);
  } else {
    shader.setUniform("material.hasNormalMap", false);
  }

  if (material.shininess_map) {
    const Texture& tex = textures[material.shininess_map.value()];
    tex.bindToTextureUnit(7);
    shader.setUniform("material.shininessMap", 7);
  }

  if (material.displacement_map) {
    const Texture& tex = textures[material.displacement_map.value()];
    tex.bindToTextureUnit(8);
    shader.setUniform("material.displacementMap", 8);

    // TODO: remove this
    shader.setUniform("material.hasDisplacementMap", true);
  } else {
    shader.setUniform("material.hasDisplacementMap", false);
  }

  if (material.light_map) {
    const Texture& tex = textures[material.light_map.value()];
    tex.bindToTextureUnit(9);
    shader.setUniform("material.lightMap", 9);

    // TODO: remove this
    shader.setUniform("material.hasLightMap", true);
  } else {
    shader.setUniform("material.hasLightMap", false);
  }

  // set material
  if (material.diffuse_map) {
    shader.setUniform("material.kd", glm::vec3(0));
  } else {
    shader.setUniform("material.kd", material.kd);
  }

  if (material.specular_map) {
    shader.setUniform("material.ks", glm::vec3(0));
  } else {
    shader.setUniform("material.ks", material.ks);
  }

  if (material.ambient_map) {
    shader.setUniform("material.ka", glm::vec3(0));
  } else {
    shader.setUniform("material.ka", material.ka);
  }

  if (material.emissive_map) {
    shader.setUniform("material.ke", glm::vec3(0));
  } else {
    shader.setUniform("material.ke", material.ke);
  }

  shader.setUniform("material.shininess", material.shininess);

  // draw mesh
  pipeline.activate();
  vao.activate();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.deactivate();
  pipeline.deactivate();

  // reset texture uniforms
  shader.setUniform("material.diffuseMap", 0);
  shader.setUniform("material.specularMap", 0);
  shader.setUniform("material.ambientMap", 0);
  shader.setUniform("material.emissiveMap", 0);
  shader.setUniform("material.heightMap", 0);
  shader.setUniform("material.hasHeightMap", false);
  shader.setUniform("material.normalMap", 0);
  shader.setUniform("material.hasNormalMap", false);
  shader.setUniform("material.shininessMap", 0);
  shader.setUniform("material.displacementMap", 0);
  shader.setUniform("material.hasDisplacementMap", false);
  shader.setUniform("material.lightMap", 0);
  shader.setUniform("material.hasLightMap", false);
}

uint32_t Mesh::getNumberOfVertices() const { return vertices.size(); }

uint32_t Mesh::getNumberOfFaces() const { return indices.size() / 3; }

}  // namespace ogls